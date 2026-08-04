// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Range/PxiiGA_Fire.h"
#include "AbilitySystemComponent.h"
#include "Character/PxiiCharacter.h"
#include "Components/PxiiCombatComponent.h"
#include "Components/PxiiPlayerCombatComponent.h"
#include "Enum/PxiiDamageType.h"
#include "GAS/PxiiAttributeSet.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Subsystem/WorldSpawnerSubsystem.h"
#include "Utility/PxiiCombatBPLibrary.h"
#include "Utility/PxiiDebugTraceBPLibrary.h"

DEFINE_LOG_CATEGORY(LogFireProjectile);
UPxiiGA_Fire::UPxiiGA_Fire()
{
    ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Pxii.Weapon.Type.Range.OneShot")));
    ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Pxii.Combat.State.ADS")));
}

void UPxiiGA_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AActor* Avatar = ActorInfo->AvatarActor.Get();
    
	SelfCharacter = Cast<APxiiCharacter>(Avatar);
	if (SelfCharacter)
	{
		APxiiWeaponBase* WeaponRef = SelfCharacter->GetWeaponRanged();
		if (WeaponRef)
		{
			UPxiiGameplayEffectCooldown* GECooldownRef = SelfCharacter->GetWeaponRanged()->GEFireRateCooldown;
			// Note(Burlin): Maybe add scaling cooldown here in the future
		}
		else
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
    
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}

		UPxiiCombatBPLibrary::StartProjectileTrace(SelfCharacter);
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UPxiiGA_Fire::RequestProjectileHit(APxiiCharacter* PxiiCharacter, const FVector& Vector, const FVector& ImpactPoint)
{
    
}

void UPxiiGA_Fire::FireProjectile(APxiiCharacter* Character)
{
    bool isHeadshot = false;
    bool DrawTraces = true;
    if (!Character) return;

    APlayerController* PC = Cast<APlayerController>(Character->GetController());
    if (!PC) return;

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

    // Get the camera location and forward direction
    FVector EndLocation;
    FVector MuzzleStartLocation;
    FRotator ViewRotation;

    //--------------------------------------------------------------------------------------------------------------------
    FVector CameraLocation;
    Character->GetController()->GetPlayerViewPoint(CameraLocation, ViewRotation);
    const FVector CameraTraceEnd = CameraLocation + (ViewRotation.Vector() * TraceDistance);
    FCollisionQueryParams CameraParams;
    CameraParams.AddIgnoredActor(Character);

    FHitResult CameraHit;
    GetWorld()->LineTraceSingleByChannel(CameraHit,
        CameraLocation,CameraTraceEnd,
        ECC_Visibility,CameraParams);

    if (DrawTraces)
    {
        UPxiiDebugTraceBPLibrary::DrawDebugArrowSimple(this,
            CameraLocation, CameraTraceEnd,
            FLinearColor::Blue, 1.f, DrawDuration   // Duration
        );
    }
    
    // 🔒 This is the TRUE crosshair world point
    const FVector AimPoint = CameraHit.bBlockingHit ? CameraHit.ImpactPoint : CameraTraceEnd;
    //--------------------------------------------------------------------------------------------------------------------
    // TODO[Dhenz]: Setup trace points for weapons
    MuzzleSocketName = "Muzzle";
    FVector TraceStart = Character->GetActorLocation();
    if (Character->GetWeaponRanged() && Character->GetWeaponRanged()->SKWeapon)
    {
        TraceStart = Character->GetWeaponRanged()->SKWeapon->GetSocketTransform(MuzzleSocketName).GetLocation();
    } else
    {
        UE_LOG(LogFireProjectile, Error, TEXT("[Firing] I Have No Weapon"));
    }
    const FVector TraceEnd = AimPoint + (AimPoint - TraceStart).GetSafeNormal() * 100.f;
    FCollisionQueryParams WeaponParams;
    WeaponParams.AddIgnoredActor(Character);

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult,
        TraceStart, TraceEnd,
        ECC_Visibility, WeaponParams);

    UPxiiDebugTraceBPLibrary::DrawDebugArrowSimple(this,
        TraceStart, TraceEnd,
        FLinearColor::Gray, 2.f, DrawDuration);

    MuzzleStartLocation = TraceStart;
    EndLocation = TraceEnd;

    //--------------------------------------------------------------------------------------------------------------------
    // TODO[ANY]: Trace Deviation Logic if any
    // Aim direction (with optional spread)
    //FVector ShootDir = FMath::VRandCone(ViewRotation.Vector(), FMath::DegreesToRadians(MaxOffsetAngleDegrees));
    //FVector AimEnd = StartLocation + (ShootDir * TraceDistance);

    //const bool bHasMuzzleObstruction = !Character->HasMuzzleObstruction;
    const bool bHasMuzzleObstruction = false;
    if (Character->GetWeaponRanged() && !bHasMuzzleObstruction) 
    {
        // TODO: Weapon Mesh and Socket
        FTransform MuzzleTransform = Character->GetWeaponRanged()->SKWeapon->GetSocketTransform(MuzzleSocketName);
        MuzzleStartLocation = MuzzleTransform.GetLocation();

        if (DrawTraces)
        {
            UPxiiDebugTraceBPLibrary::DrawDebugSphereSimple(this, MuzzleStartLocation, 
                   3.f, FColor::Blue, DrawDuration);
        }
    }
    //--------------------------------------------------------------------------------------------------------------------
    //FVector EndLocation = (AimEnd - MuzzleStartLocation).GetSafeNormal();
    MaxOffsetAngleDegrees = 0;
    //FVector DeviatedDirection = FMath::VRandCone(ViewRotation.Vector(), FMath::DegreesToRadians(MaxOffsetAngleDegrees));
    //FVector EndLocation = StartLocation + (DeviatedDirection * TraceDistance);
    
    // Collision query params
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(Character);

    FHitResult OutHitResult;

    bool bHit = false;

    // ---- Single Line Trace ----
    bHit = GetWorld()->LineTraceSingleByChannel(
        OutHitResult,
        MuzzleStartLocation, EndLocation,
        ECC_Visibility,
        TraceParams
    );

    //--------------------------------------------------------------------------------------------------------------------
    UE_LOG(LogFireProjectile, Warning, TEXT("---------------- I Should Fire Here Process X1"));
    // TODO: Burlin
    // ---- Server authority logic ----
    if (APxiiCharacter* MainCharacter = Cast<APxiiCharacter>(Character))
    {
        UPxiiCombatComponent* SelfCombatComp = IPxiiCombatInterface::Execute_GetCombatComponent(MainCharacter);
        if (!SelfCombatComp) return;
        if (MainCharacter->GetIsObstructed())  return;
        
        UE_LOG(LogFireProjectile, Warning, TEXT("---------------- Process Trace"));
        if (bHit)
        {
            isHeadshot = OutHitResult.BoneName == FName("head");
            UE_LOG(LogTemp, Warning, TEXT("- Hit Actor: %s | Bone: %s :: %s"), *GetNameSafe(OutHitResult.GetActor()), *OutHitResult.BoneName.ToString(), isHeadshot ? TEXT("Headshot") : TEXT("Normal"));

            if (UWorldSpawnerSubsystem* Spawner = MainCharacter->GetWorld()->GetSubsystem<UWorldSpawnerSubsystem>())
            {
                FVector spawnLoc = OutHitResult.GetActor()->GetActorLocation() + FVector(0.f, 0.f, 100.f);
                FText NewText = FText::FromString(TEXT("HeadShot!"));
                UE_LOG(LogFireProjectile, Warning, TEXT("---------------- HEAD SHOT Trace"));
                Spawner->OnSpawnMessageText.Broadcast(spawnLoc, NewText, FColor::Red);
            }
            else
            {
                UE_LOG(LogFireProjectile, Warning, TEXT("---------------- HEAD SHOT FAIL"));
            }

            if (DrawTraces)
            {
                // Trace Line towards impact point
                UPxiiDebugTraceBPLibrary::DrawDebugArrowSimple(this,
                    MuzzleStartLocation, OutHitResult.ImpactPoint,
                    FLinearColor::Green, 1.f, DrawDuration);

                // Trace impact point
                UPxiiDebugTraceBPLibrary::DrawDebugSphereSimple(this,
                       OutHitResult.ImpactPoint, 
                       7.f, FColor::Cyan, DrawDuration);
            }
            if (!bUseSphereTrace)
            {
                SelfCombatComp->TriggerProjectileTrace(OutHitResult.ImpactNormal, EndLocation);
            }
            if (OutHitResult.GetActor())
            {
                //TODO[BURLIN]: Hit TraceLogic without Server
                UE_LOG(LogFireProjectile, Warning, TEXT("---------------- I HIT: {%s}"), *OutHitResult.GetActor()->GetName());

                AActor* CurrHitActor = OutHitResult.GetActor();
                const ACharacter* CharHitRef = Cast<ACharacter>(CurrHitActor);
                if (const bool ImplementsCombatInterface = CurrHitActor->GetClass()->ImplementsInterface(UPxiiCombatInterface::StaticClass()))
                {
                    //const bool IsBossUnit = UPxiiCombatInterface::Execute_IsBossUnit(CurrHitActor);
                    const bool IsBossUnit = false;
                    if (IsBossUnit)
                    {
                        // Process Body Part
                    }
                    else
                    {
                        if (UPxiiPlayerCombatComponent* PlayerCombatComp = Cast<UPxiiPlayerCombatComponent>(SelfCombatComp))
                        {
                            PlayerCombatComp->ProcessUnitDamage(CurrHitActor, OutHitResult.ImpactPoint, 5.f, EDamageSource::Range);
                        }
                        const UPxiiAttributeSet* AttributeSet = IPxiiCombatInterface::Execute_GetAttributeSet(CurrHitActor);
                        UE_LOG(LogFireProjectile, Warning, TEXT("---------------- I Damage: {%f}"), AttributeSet->Health.GetCurrentValue());
                    }
                }
            }
        }
        else
        {
            // MISSED, Hit no Valid Target

            UE_LOG(LogFireProjectile, Warning, TEXT("---------------- I MISS"));
            
            //MainCharacter->HitValidTarget.Broadcast(false);
            if (DrawTraces)
            {
                // Trace Line towards impact point
                UPxiiDebugTraceBPLibrary::DrawDebugArrowSimple(this,
                    MuzzleStartLocation, EndLocation,
                    FLinearColor::Red, 1.f, DrawDuration);

                // Trace impact point
                UPxiiDebugTraceBPLibrary::DrawDebugSphereSimple(this,
                       EndLocation,
                       7.f, FColor::Cyan, DrawDuration);
            }
            
            if (bUseSphereTrace)
            {
                FVector Start = MuzzleStartLocation;
                FVector End = EndLocation;

                float Distance = FVector::Dist(Start, OutHitResult.ImpactPoint);

                // Scale height: 500 height per 1000 units
                float ArcHeight = (Distance / 1000.f) * 250.f;

                // Clamp if you don’t want absurdly tall arcs
                ArcHeight = FMath::Clamp(ArcHeight, 200.f, 3000.f);

                SelfCombatComp->TriggerProjectileTraceArc(OutHitResult.ImpactNormal, EndLocation, ArcHeight);
            }
            else
            {
                SelfCombatComp->TriggerProjectileTrace(OutHitResult.ImpactNormal, EndLocation);
            }
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
        UE_LOG(LogFireProjectile, Warning, TEXT("---------------- FAILED TO FIRE"));
    }
}
