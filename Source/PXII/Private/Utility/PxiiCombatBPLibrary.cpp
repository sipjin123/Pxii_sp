// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/PxiiCombatBPLibrary.h"

#include "Character/PxiiNPC.h"
#include "Settings/CombatDeveloperSettings.h"
#include "Utility/PxiiDebugTraceBPLibrary.h"
#include "Utility/PXIILogUtility.h"
#include "Interface/PxiiCombatInterface.h"
#include "Components/PxiiPlayerCombatComponent.h"
#include "Data/PxiiTags.h"
#include "Enum/PxiiDamageType.h"
#include "Enum/PxiiEnemyType.h"
#include "Interface/PxiiDamageableInterface.h"
#include "Subsystem/PxiiCombatRegistrySubsystem.h"
#include "Subsystem/WorldSpawnerSubsystem.h"

bool UPxiiCombatBPLibrary::GetWeaponSocketTransform(APxiiCharacter* character, FName MuzzleSocketName, FTransform& OutTransform)
{
    if(!character)
    {
        return false;
    }
    if (character->Implements<UPxiiCombatInterface>())
    {
        APxiiWeaponRange* RangeWeapon = IPxiiCombatInterface::Execute_GetWeaponBaseRange(character);
        if (RangeWeapon && RangeWeapon->SKWeapon)
        {
            OutTransform = RangeWeapon->SKWeapon->GetSocketTransform(MuzzleSocketName);
            return true;
        }
    }

    return false;
}

void UPxiiCombatBPLibrary::RegisterHitEffect(AActor* SourceActor, AActor* TargetActor, const FHitResult& result, float Magnitude, EHitEffectType HitEffectType)
{
    UAbilitySystemComponent* InstigatorASC = nullptr;
    if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(SourceActor))
    {
        InstigatorASC = ASI->GetAbilitySystemComponent();
        FGameplayEffectContextHandle ContextHandle = InstigatorASC->MakeEffectContext();
        ContextHandle.AddSourceObject(SourceActor);
        ContextHandle.AddHitResult(result);
        ContextHandle.AddOrigin(result.ImpactPoint);
        ContextHandle.AddInstigator(SourceActor, SourceActor);
		
        IAbilitySystemInterface* TargetASI = Cast<IAbilitySystemInterface>(TargetActor);
        if (!TargetASI)
        {
            return;
        }

        UAbilitySystemComponent* TargetASC = TargetASI->GetAbilitySystemComponent();
        if (!TargetASC)
        {
            return;
        }
		
        if (UPxiiCombatRegistrySubsystem* Spawner = SourceActor->GetWorld()->GetSubsystem<UPxiiCombatRegistrySubsystem>())
        {
            const EFactionType SourceFaction = IPxiiCombatInterface::Execute_GetFaction(SourceActor);
            const EFactionType TargetFaction = IPxiiCombatInterface::Execute_GetFaction(TargetActor);

            if (SourceFaction != TargetFaction){
                FGameplayEffectSpecHandle SpecHandle = InstigatorASC->MakeOutgoingSpec(Spawner->GetGenericDamageEffect(), 1.0f, ContextHandle);
                if(SpecHandle.IsValid())
                {
                    // Add the magnitude value as a tag with payload.
                    const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Combat.Damage"));
                    SpecHandle.Data->SetSetByCallerMagnitude(DamageTag, Magnitude);

                    InstigatorASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
                }
                RegisterHitStatusEffectOnly(SourceActor, TargetActor, result, Magnitude, HitEffectType);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("---------------- FAILED TO Register Hit"));
    }
}

void UPxiiCombatBPLibrary::RegisterHitStatusEffectOnly(AActor* SourceActor, AActor* TargetActor, const FHitResult& result, float Magnitude, EHitEffectType HitEffectType)
{
    if (true)//if(HitEffectType != EHitEffectType::Default)
    {
        UE_LOG(LogTemp, Warning, TEXT("Should Apply Infliction: %s"), *StaticEnum<EHitEffectType>()->GetNameStringByValue(static_cast<int64>(HitEffectType)));
        FHitResult HitResult;
        HitResult.ImpactPoint = result.ImpactPoint;
        HitResult.Location = result.Location;
        FGameplayEventData Payload;

        FGameplayAbilityTargetDataHandle TargetDataHandle;
        TargetDataHandle.Add(
            new FGameplayAbilityTargetData_SingleTargetHit(HitResult)
        );

        Payload.TargetData = TargetDataHandle;
        Payload.Instigator = SourceActor;
        
        FGameplayTag DynamicTag = FGameplayTag::RequestGameplayTag(FName("Abilities.Character.Infliction.Flinch"));
        switch (HitEffectType)
        {
        case EHitEffectType::Knockback:
            DynamicTag = FGameplayTag::RequestGameplayTag(FName("Abilities.Character.Infliction.Knockback"));
            break;
        case EHitEffectType::Knockdown:
            DynamicTag = FGameplayTag::RequestGameplayTag(FName("Abilities.Character.Infliction.Knockdown"));
            break;
        case EHitEffectType::FlyAway:
            DynamicTag = FGameplayTag::RequestGameplayTag(FName("Abilities.Character.Infliction.FlyAway"));
            break;
        case EHitEffectType::Stagger:
            DynamicTag = FGameplayTag::RequestGameplayTag(FName("Abilities.Character.Infliction.Stagger"));
            break;
        case EHitEffectType::Ministun:
            DynamicTag = FGameplayTag::RequestGameplayTag(FName("Abilities.Character.Infliction.Ministun"));
            break;
        case EHitEffectType::Flinch:
            DynamicTag = FGameplayTag::RequestGameplayTag(FName("Abilities.Character.Infliction.Flinch"));
            break;
        }
        Payload.EventTag = DynamicTag;

        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, DynamicTag, Payload);
    }
}

TArray<AActor*> UPxiiCombatBPLibrary::GetActorsWithinRadius(FVector Origin, float Radius, UObject* WorldContextObject,
                                                            EFactionType FactionType, bool ShowDebug)
{
    const UWorld * world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
    TArray<AActor*> OutActors;
    const FCollisionShape DetectionSphere = FCollisionShape::MakeSphere(Radius);

    // Define object types to detect (only detect Pawns, like enemies)
    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

    // Store detected actors
    TArray<FOverlapResult> OverlapResults;

    // Perform Overlap
    const bool bHasOverlap = world->OverlapMultiByObjectType(
        OverlapResults, 
        Origin, 
        FQuat::Identity, 
        ObjectQueryParams, 
        DetectionSphere
    );

    // Draw debug sphere (DEBUG)
    if(ShowDebug)
    {
        DrawDebugSphere(world, Origin, Radius, 16, FColor::Green, false, 2.0f);
    }
	
    // Use a set to track unique actors
    TSet<AActor*> UnitsHit;
	
    // Process detected actors
    if (bHasOverlap)
    {
        for (const FOverlapResult& Result : OverlapResults)
        {
            AActor* DetectedActor = Result.GetActor();
            if (DetectedActor)
            {
                if (DetectedActor->Implements<UPxiiDamageableInterface>())
                {
                    UE_LOG(LogTemp, Warning, TEXT("Part Enemy detected: %s"), *DetectedActor->GetName());
                }
                if (DetectedActor->Implements<UPxiiCombatInterface>())
                {
                    bool isAlive = true; //IPxiiCombatInterface::Execute_IsAlive(DetectedActor)
                    if (isAlive)
                    {
                        // Filter actors here
                        bool IsValidActorHit =
                                FactionType == EFactionType::Player && DetectedActor->IsA(APxiiCharacter::StaticClass()) || 
                                FactionType == EFactionType::Enemy && DetectedActor->IsA(APxiiNPC::StaticClass()) ||
                                FactionType == EFactionType::Neutral && DetectedActor->IsA(ACharacter::StaticClass());
			
                        //bool IsValidActorHit = true;

                        // Check validity and prevent duplicates
                        if (IsValidActorHit && !UnitsHit.Contains(DetectedActor))
                        {
                            if(ShowDebug)
                            {
                                // Log Collision (DEBUG)
                                UE_LOG(LogTemp, Warning, TEXT("Enemy detected: %s"), *DetectedActor->GetName());
                            }
                            UnitsHit.Add(DetectedActor);
                        }
                    }
                }
            }
        }
    }

    return UnitsHit.Array();
}

void UPxiiCombatBPLibrary::StartProjectileTrace(APxiiCharacter* Character, FHitInformation& TraceInformation, bool processDamage, FName MuzzleSocketName, bool drawDebugTrace)
{
    bool DrawTraces = drawDebugTrace;
    if (!Character)
    {
        return;
    }
    
    UWorld* World = Character->GetWorld();
    if(!World)
    {
        return;
    }
    
    APlayerController* PC = Cast<APlayerController>(Character->GetController());
    if (!PC)
    {
        return;
    }
    // Fire mode toggle
    bool bUseSphereTrace = false;
    if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent())
    {
        // TODO[ANY]: If the projectile hits any target within AOE
        /*
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Buff.Homing"))))
        {
            bUseSphereTrace = true;
        }*/
    }

    constexpr float TraceDistance = 10000.f;
    float MaxOffsetAngleDegrees = 1.0f; // tweak for spread
    
    FHitResult CameraHit;
    FVector CameraTraceEnd;
    DoCameraTrace(Character, TraceDistance, CameraHit, CameraTraceEnd, DrawTraces);
    
    // 🔒 This is the TRUE crosshair world point
    FHitInformation SocketHitInfo;
    FVector AimPoint = CameraHit.bBlockingHit ? CameraHit.ImpactPoint : CameraTraceEnd;
    bool hit = DoSocketTrace(Character, MuzzleSocketName, AimPoint, SocketHitInfo, DrawTraces);
    
    FHitResult SocketHit = SocketHitInfo.HitResult;
    TraceInformation = SocketHitInfo;
    
    if (APxiiCharacter* MainCharacter = Cast<APxiiCharacter>(Character))
    {
        if (MainCharacter->GetIsObstructed())
        {
            return;   
        }
        
        if (hit)
        {
            ProcessTraceHit(MainCharacter, SocketHit, processDamage, DrawTraces);
        }
        else
        {
            ProcessTraceMissed(MainCharacter, SocketHitInfo, DrawTraces);
        }

        // TODO[BURLIN]: Use this for Destructibles
        /*
        if (OutHitResult.GetComponent())
        {
            UGeometryCollectionComponent* GCComp = Cast<UGeometryCollectionComponent>(OutHitResult.GetComponent());
            if (GCComp)
            {
                MainCharacter->HitFracture.Broadcast(OutHitResult.ImpactPoint, GCComp);
            }
        }*/
    } else
    {
        UE_LOG(LogTemp, Warning, TEXT("---------------- FAILED TO FIRE"));
    }
}

void UPxiiCombatBPLibrary::ProcessTraceHit(APxiiCharacter* character, FHitResult HitResult, bool processDamage, bool DrawTrace)
{
    UPxiiCombatComponent* SelfCombatComp = IPxiiCombatInterface::Execute_GetCombatComponent(character);
    if (!SelfCombatComp)
    {
        return;   
    }

    bool isHeadshot = HitResult.BoneName == FName("head");
    PXII_LOG(ELogCategory::Trace, Warning, TEXT("Hit Actor: %s | Bone: %s :: %s"), *GetNameSafe(HitResult.GetActor()), *HitResult.BoneName.ToString(), isHeadshot ? TEXT("Headshot") : TEXT("Normal"));

    if (isHeadshot)
    {
        if (UWorldSpawnerSubsystem* Spawner = character->GetWorld()->GetSubsystem<UWorldSpawnerSubsystem>())
        {
            FVector spawnLoc = HitResult.GetActor()->GetActorLocation() + FVector(0.f, 0.f, 100.f);
            FText NewText = FText::FromString(TEXT("HeadShot!"));
            Spawner->OnSpawnMessageText.Broadcast(spawnLoc, NewText, FColor::Red);
        }
    }

    if(!processDamage)
    {
        return;
    }
    
    if (HitResult.GetActor())
    {
        PXII_LOG(ELogCategory::Trace, Warning, TEXT("TRACE HIT: {%s}"), *HitResult.GetActor()->GetName());

        AActor* CurrHitActor = HitResult.GetActor();
        const ACharacter* CharHitRef = Cast<ACharacter>(CurrHitActor);
        if (const bool ImplementsCombatInterface = CurrHitActor->GetClass()->ImplementsInterface(UPxiiCombatInterface::StaticClass()))
        {
            UE_LOG(LogTemp, Warning, TEXT("---------------- Hit Entity"));
            //const bool IsBossUnit = UPxiiCombatInterface::Execute_IsBossUnit(CurrHitActor);
            const bool IsBossUnit = false;
            if (IsBossUnit)
            {

            }
            else
            {
                if (UPxiiPlayerCombatComponent* PlayerCombatComp = Cast<UPxiiPlayerCombatComponent>(SelfCombatComp))
                {
                    PlayerCombatComp->ProcessUnitDamage(CurrHitActor, HitResult.ImpactPoint, 5.f,  EDamageSource::Range);
                }
                const UPxiiAttributeSet* AttributeSet = IPxiiCombatInterface::Execute_GetAttributeSet(CurrHitActor);
                PXII_LOG(ELogCategory::Trace, Warning, TEXT("TRACE HIT Damage: {%f}"), AttributeSet->Health.GetCurrentValue());
            }
        }

        if (const bool ImplementsDamageableInterface = CurrHitActor->GetClass()->ImplementsInterface(UPxiiDamageableInterface::StaticClass()))
        {
            UE_LOG(LogTemp, Warning, TEXT("---------------- Hit BODY PART"));
        }
    }
}

void UPxiiCombatBPLibrary::ProcessTraceMissed(APxiiCharacter* character, FHitInformation TraceInfo, bool DrawTrace)
{
    UPxiiCombatComponent* SelfCombatComp = IPxiiCombatInterface::Execute_GetCombatComponent(character);
    if (!SelfCombatComp)
    {
        return;   
    }
    PXII_LOG(ELogCategory::Trace, Log, TEXT("NO Trace HIT"));
    
    FHitResult SocketHit = TraceInfo.HitResult;
    bool bUseSphereTrace = false;
    if (bUseSphereTrace)
    {
        FVector Start = TraceInfo.TraceStart;
        FVector End = TraceInfo.TraceEnd;

        float Distance = FVector::Dist(Start, SocketHit.ImpactPoint);

        // Scale height: 500 height per 1000 units
        float ArcHeight = (Distance / 1000.f) * 250.f;

        // Clamp if you don’t want absurdly tall arcs
        ArcHeight = FMath::Clamp(ArcHeight, 200.f, 3000.f);

        SelfCombatComp->TriggerProjectileTraceArc(SocketHit.ImpactNormal, End, ArcHeight);
    }
    else
    {
        SelfCombatComp->TriggerProjectileTrace(SocketHit.ImpactNormal, TraceInfo.TraceEnd, DrawTrace);
    }
}

bool UPxiiCombatBPLibrary::DoCameraTrace(APxiiCharacter* character, float TraceDistance, FHitResult& HitResult, FVector& TraceEnd, bool DrawTrace)
{
    if(!character)
    {
        return false;
    }
    
    FVector CameraLocation;
    FVector CameraDirection;
    
    if(!GetCameraViewPoint(character, CameraLocation, CameraDirection))
    {
        return false;
    }

    UPxiiAimAssistComponent* aimAssistComp = IPxiiCombatInterface::Execute_GetAimAssistComponent(character);
    FVector assistedAimDir = CameraDirection;
    if (aimAssistComp)
    {
        assistedAimDir = aimAssistComp->GetAdjustedAimDirection(CameraDirection, CameraLocation);
    }

    PXII_LOG(ELogCategory::Aim, Log, TEXT("Raw Aim Direction: %s ===> Assisted Aim Direction: %s"), *CameraDirection.ToString(), *assistedAimDir.ToString());
    TraceEnd = CameraLocation + (assistedAimDir * TraceDistance);
    FCollisionQueryParams CameraParams;
    CameraParams.AddIgnoredActor(character);
    
    UWorld* world = character->GetWorld();
    if(!world)
    {
        return false;
    }
    
    if (DrawTrace)
    {
        UPxiiDebugTraceBPLibrary::DrawDebugArrowSimple(world,CameraLocation, TraceEnd, FLinearColor::Blue,
            1.f, 1.0f);
    }

    return world->LineTraceSingleByChannel(HitResult,CameraLocation, TraceEnd,ECC_Visibility, CameraParams);
}

bool UPxiiCombatBPLibrary::IsGameWorld(UObject* WorldContextObject)
{
    if (!WorldContextObject) return false;
    const UWorld* World = WorldContextObject->GetWorld();
    return World && World->IsGameWorld();
}

bool UPxiiCombatBPLibrary::DoSocketTrace(APxiiCharacter* character, FName socketName, FVector aimPoint, FHitInformation& HitResult, bool DrawTrace)
{
    float debugDuration = 1.0f;
    if(!character)
    {
        return false;
    }
    
    FVector TraceStart = character->GetActorLocation();
    if (character->Implements<UPxiiCombatInterface>())
    {
        APxiiWeaponRange* RangeWeapon = IPxiiCombatInterface::Execute_GetWeaponBaseRange(character);
        if (RangeWeapon && RangeWeapon->SKWeapon)
        {
            TraceStart = RangeWeapon->SKWeapon->GetSocketTransform(socketName).GetLocation();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[Firing] I Have No Weapon"));
        }
    }

    UWorld* world = character->GetWorld();
    if(!world)
    {
        return false;
    }
    
    const FVector TraceEnd = aimPoint + (aimPoint - TraceStart).GetSafeNormal() * 100.f;
    FCollisionQueryParams WeaponParams;
    WeaponParams.bReturnPhysicalMaterial = true;
    WeaponParams.AddIgnoredActor(character);
    
    FHitResult HitData;
    bool isHit = world->LineTraceSingleByChannel(HitData, TraceStart, TraceEnd, ECC_Visibility, WeaponParams);
    
    if(DrawTrace)
    {
        UPxiiDebugTraceBPLibrary::DrawDebugArrowSimple(world, TraceStart, TraceEnd, FLinearColor::Gray, 2.f, debugDuration);

        if(isHit)
        {
            // Trace Line towards impact point
            UPxiiDebugTraceBPLibrary::DrawDebugArrowSimple(world, TraceStart, HitData.ImpactPoint, FLinearColor::Green, 1.f, debugDuration);
            // Trace impact point
            UPxiiDebugTraceBPLibrary::DrawDebugSphereSimple(world, HitData.ImpactPoint, 7.f, FColor::Cyan, debugDuration);
        }
        else
        {
            // Trace Line towards impact point
            UPxiiDebugTraceBPLibrary::DrawDebugArrowSimple(world, TraceStart, TraceEnd, FLinearColor::Red, 1.f, debugDuration);

            // Trace impact point
            UPxiiDebugTraceBPLibrary::DrawDebugSphereSimple(world, TraceEnd,7.f, FColor::Cyan, debugDuration);
        }
    }
    FVector TraceDir = (TraceEnd - TraceStart).GetSafeNormal();

    HitResult = FHitInformation(HitData, TraceStart, TraceEnd, TraceDir);
    return isHit;
}

bool UPxiiCombatBPLibrary::GetCameraViewPoint(APxiiCharacter* character, FVector& OutLoc, FVector& OutDir)
{
    if (!character)
    {
        return false;   
    }
 
    APlayerController* PC = Cast<APlayerController>(character->GetController());
    if (!PC || !PC->PlayerCameraManager)
    {
        return false;
    }
    
    OutLoc = PC->PlayerCameraManager->GetCameraLocation();
    OutDir = PC->PlayerCameraManager->GetCameraRotation().Vector();

    return true;
}
TArray<AActor*> UPxiiCombatBPLibrary::MultiSphereTraceTargetChain(UObject* WorldContextObject, AActor* Owner, FVector Origin,FVector Direction,float Distance,float SphereRadius,int32 MaxTraces,TSubclassOf<AActor> TargetActorClass,ETraceTypeQuery TraceChannel,bool bDrawDebug,bool bErrorLog)
{
    TArray<AActor*> FoundActors;
    if (!WorldContextObject||Distance<=0.f||SphereRadius<=0.f||MaxTraces<=0)
    {
        if (bErrorLog) UE_LOG(LogTemp,Warning,TEXT("TargetChain FAILED: Invalid parameters"));
        return FoundActors;
    }
    Direction=Direction.GetSafeNormal();
    if (Direction.IsNearlyZero())
    {
        if (bErrorLog) UE_LOG(LogTemp,Warning,TEXT("TargetChain FAILED: Direction is zero"));
        return FoundActors;
    }
    UWorld* World=GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::ReturnNull);
    if (!World)
    {
        if (bErrorLog) UE_LOG(LogTemp,Warning,TEXT("TargetChain FAILED: World is null"));
        return FoundActors;
    }
    FVector CurrentStart=Origin;
    float RemainingDistance=Distance;
    TSet<AActor*> UniqueActors;
    for (int32 TraceIndex=0;TraceIndex<MaxTraces&&RemainingDistance>0.f;++TraceIndex)
    {
        const FVector CurrentEnd=CurrentStart+Direction*RemainingDistance;
        TArray<FHitResult> Hits;
        TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(WorldContextObject->GetTypedOuter<AActor>());
        ActorsToIgnore.Add(Owner);
        for (AActor* Actor:UniqueActors)
        {
            ActorsToIgnore.Add(Actor);
        }
        UKismetSystemLibrary::SphereTraceMulti(World,CurrentStart,CurrentEnd,SphereRadius,TraceChannel,false,ActorsToIgnore,bDrawDebug?EDrawDebugTrace::ForDuration:EDrawDebugTrace::None,Hits,true);
        if (Hits.Num()==0)
        {
            if (bErrorLog) UE_LOG(LogTemp,Warning,TEXT("Trace[%d] STOP: No collision"),TraceIndex);
            break;
        }
        Hits.Sort([](const FHitResult& A,const FHitResult& B)
        {
            return A.Distance<B.Distance;
        });
        AActor* HitActor=nullptr;
        float HitDistance=RemainingDistance;
        for (const FHitResult& Hit:Hits)
        {
            AActor* Actor=Hit.GetActor();
            if (!IsValid(Actor))
            {
                if (bErrorLog) UE_LOG(LogTemp,Warning,TEXT("Trace[%d] Ignored: Invalid actor"),TraceIndex);
                continue;
            }
            if (UniqueActors.Contains(Actor))
            {
                if (bErrorLog) UE_LOG(LogTemp,Warning,TEXT("Trace[%d] Ignored: Already hit %s"),TraceIndex,*GetNameSafe(Actor));
                continue;
            }
            if (TargetActorClass&&!Actor->IsA(TargetActorClass))
            {
                if (bErrorLog) UE_LOG(LogTemp,Warning,TEXT("Trace[%d] Ignored: %s is not TargetPawnClass"),TraceIndex,*GetNameSafe(Actor));
                continue;
            }
            HitActor=Actor;
            HitDistance=Hit.Distance;
            break;
        }
        if (!HitActor)
        {
            if (bErrorLog) UE_LOG(LogTemp,Warning,TEXT("Trace[%d] STOP: No valid target found"),TraceIndex);
            break;
        }
        UniqueActors.Add(HitActor);
        FoundActors.Add(HitActor);
        RemainingDistance-=HitDistance;
        if (RemainingDistance<=0.f)
        {
            if (bErrorLog) UE_LOG(LogTemp,Warning,TEXT("Trace[%d] STOP: Distance exhausted"),TraceIndex);
            break;
        }
        CurrentStart=CurrentStart+Direction*HitDistance;
    }
    return FoundActors;
}

APawn* UPxiiCombatBPLibrary::GetPawnInFrontByBoxTrace(UObject* WorldContextObject, AActor* SourceActor,
    FVector BoxExtent, FVector Offset, float Distance, ETraceTypeQuery TraceChannel, bool bDrawDebug)
{
    if (!SourceActor||Distance<=0.f)
    {
        return nullptr;
    }
    UWorld* World=WorldContextObject->GetWorld();
    if (!World)
    {
        return nullptr;
    }
    const FVector Start=SourceActor->GetActorLocation() + Offset;
    const FVector Direction=SourceActor->GetActorForwardVector().GetSafeNormal();
    if (Direction.IsNearlyZero())
    {
        return nullptr;
    }
    const FVector End=Start+Direction*Distance;
    FHitResult Hit;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(SourceActor);
    const bool bHit=UKismetSystemLibrary::BoxTraceSingle(World,Start,End,BoxExtent,SourceActor->GetActorRotation(),TraceChannel,false,ActorsToIgnore,bDrawDebug?EDrawDebugTrace::ForDuration:EDrawDebugTrace::None,Hit,true);
    if (!bHit)
    {
        return nullptr;
    }
    return Cast<APawn>(Hit.GetActor());
}

TArray<int32> UPxiiCombatBPLibrary::GenerateSquadAttackTypes(int32 AttackerCount)
{
    TArray<int32> AttackTypes;
    if (AttackerCount<=0)
    {
        return AttackTypes;
    }
    AttackTypes.Reserve(AttackerCount);
    for (int32 i=0;i<AttackerCount;++i)
    {
        const int32 Roll=FMath::RandRange(0,99);
        if (Roll<60)
        {
            AttackTypes.Add(0);
        }
        else if (Roll<90)
        {
            AttackTypes.Add(1);
        }
        else
        {
            AttackTypes.Add(2);
        }
    }
    if (AttackerCount>1)
    {
        bool bHasSpecialOrPowerful=false;
        for (const int32 AttackType : AttackTypes)
        {
            if (AttackType==1||AttackType==2)
            {
                bHasSpecialOrPowerful=true;
                break;
            }
        }
        if (!bHasSpecialOrPowerful)
        {
            const int32 Index=FMath::RandRange(0,AttackTypes.Num()-1);
            AttackTypes[Index]=FMath::RandBool()?1:2;
        }
    }
    return AttackTypes;
}
