// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/PxiiCombatBPLibrary.h"
#include "Settings/CombatDeveloperSettings.h"
#include "Utility/PxiiDebugTraceBPLibrary.h"
#include "Utility/PXIILogUtility.h"
#include "Interface/PxiiCombatInterface.h"
#include "Components/PxiiPlayerCombatComponent.h"
#include "Data/PxiiTags.h"
#include "Enum/PxiiDamageType.h"
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
