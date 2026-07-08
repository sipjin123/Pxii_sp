// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Range/PxiiGA_Fire.h"

#include "AbilitySystemComponent.h"
#include "Character/PxiiCharacter.h"
#include "Components/PxiiCombatComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Utility/PxiiDebugTraceBPLibrary.h"

DEFINE_LOG_CATEGORY(LogFireProjectile);
UPxiiGA_Fire::UPxiiGA_Fire()
{
}

void UPxiiGA_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AActor* Avatar = ActorInfo->AvatarActor.Get();
    
	APxiiCharacter* Character = Cast<APxiiCharacter>(Avatar);
	if (Character)
	{
		APxiiWeaponBase* WeaponRef = Character->GetWeaponRanged();
		if (WeaponRef)
		{
			UPxiiGameplayEffectCooldown* GECooldownRef = Character->GetWeaponRanged()->GEFireRateCooldown;
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

		FireProjectile(Character);
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UPxiiGA_Fire::FireProjectile(APxiiCharacter* Character)
{
    bool DrawTraces = true;
    if (!Character) return;

    // Fire mode toggle
    bool bUseSphereTrace = false;
    if (UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent())
    {
        /*
        if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Buff.Homing"))))
        {
            bUseSphereTrace = true;
        }*/
    }

    constexpr float TraceDistance = 10000.f;
    float MaxOffsetAngleDegrees = 1.0f; // tweak for spread

    // Get the camera location and forward direction
    //FVector StartLocation;
    FVector EndLocation;
    FVector MuzzleStartLocation;
    FRotator ViewRotation;

    APlayerController* PC = Cast<APlayerController>(Character->GetController());
    if (!PC)
    {
        return;
    }

    //--------------------------------------------------------------------------------------------------------------------

    
    FVector CameraLocation;

    Character->GetController()->GetPlayerViewPoint(
        CameraLocation,
        ViewRotation
    );

    const FVector CameraTraceEnd =
        CameraLocation + (ViewRotation.Vector() * TraceDistance);

    FCollisionQueryParams CameraParams;
    CameraParams.AddIgnoredActor(Character);

    FHitResult CameraHit;
    GetWorld()->LineTraceSingleByChannel(
        CameraHit,
        CameraLocation,
        CameraTraceEnd,
        ECC_Visibility,
        CameraParams
    );

    // 🔒 This is the TRUE crosshair world point
    const FVector AimPoint =
        CameraHit.bBlockingHit ? CameraHit.ImpactPoint : CameraTraceEnd;


    // TODO[Dhenz]: Setup trace points for weapons
    MuzzleSocketName = "Muzzle";
    FVector TraceStart = Character->GetActorLocation();

    if (Character->GetWeaponRanged())
    {
        UE_LOG(LogFireProjectile, Warning, TEXT("---------------- I Have Weapon"));
        if (Character->GetWeaponRanged()->SKWeapon){
            UE_LOG(LogFireProjectile, Warning, TEXT("---------------- I Have Weapon SK"));
            TraceStart = Character->GetWeaponRanged()->SKWeapon->GetSocketTransform(MuzzleSocketName).GetLocation();
        }
    }else
    {
        UE_LOG(LogFireProjectile, Warning, TEXT("---------------- I Have NNOOO Weapon"));
    }
    //const FVector TraceStart = Character->GetActorLocation();
    
    //const FVector TraceEnd = AimPoint; This will fail because its exact distance
    const FVector TraceEnd = AimPoint + (AimPoint - TraceStart).GetSafeNormal() * 100.f;
    FCollisionQueryParams WeaponParams;
    WeaponParams.AddIgnoredActor(Character);

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(
        HitResult,
        TraceStart,
        TraceEnd,
        ECC_Visibility,
        WeaponParams
    );

    UPxiiDebugTraceBPLibrary::DrawDebugArrowSimple(this,
        TraceStart, TraceEnd,
        FLinearColor::Gray, 3.f);

    MuzzleStartLocation = TraceStart;
    EndLocation = TraceEnd;




    //--------------------------------------------------------------------------------------------------------------------
    // Aim direction (with optional spread)
    //FVector ShootDir = FMath::VRandCone(ViewRotation.Vector(), FMath::DegreesToRadians(MaxOffsetAngleDegrees));
    //FVector AimEnd = StartLocation + (ShootDir * TraceDistance);

    //const bool bHasMuzzleObstruction = !Character->HasMuzzleObstruction;
    const bool bHasMuzzleObstruction = false;
    if (Character->GetWeaponRanged() && !bHasMuzzleObstruction) 
    {
        // TODO: Weapon Mesh and Socket
        /*
        FTransform MuzzleTransform = Character->GetWeaponBase()->SMWeapon->GetSocketTransform(MuzzleSocketName);
        MuzzleStartLocation = MuzzleTransform.GetLocation();
        */

        if (DrawTraces)
        {
            UPxiiDebugTraceBPLibrary::DrawDebugSphereSimple(this,
                   MuzzleStartLocation, 
                   3.f, FColor::Blue, 3.f
               );
        }
    }
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
        MuzzleStartLocation,
        EndLocation,
        ECC_Visibility,
        TraceParams
    );


    UE_LOG(LogFireProjectile, Warning, TEXT("---------------- I Should Fire Here"));
    // TODO: Burlin
    // ---- Server authority logic ----
    if (APxiiCharacter* MainCharacter = Cast<APxiiCharacter>(Character))
        {
            if (MainCharacter->GetIsObstructed())
            {
                return;
            }
            
            if (bHit)
            {
                UE_LOG(LogFireProjectile, Warning, TEXT("---------------- I HIT"));
                if (DrawTraces)
                {
                    UPxiiDebugTraceBPLibrary::DrawDebugArrowSimple(this,
                        MuzzleStartLocation, OutHitResult.ImpactPoint,
                        FLinearColor::Green, 3.f);
    
                    UPxiiDebugTraceBPLibrary::DrawDebugSphereSimple(this,
                           OutHitResult.ImpactPoint, 
                           7.f, FColor::Cyan, 3.f);
                }
                if (!bUseSphereTrace)
                {
                    if (UPxiiCombatComponent* CombatComp = IPxiiCombatInterface::Execute_GetCombatComponent(MainCharacter))
                    {
                        CombatComp->TriggerProjectileTrace(OutHitResult.ImpactNormal, EndLocation);
                    }
                }
                if (OutHitResult.GetActor())
                {
                    //TODO[BURLIN]: Hit TraceLogic without Server
                    //RequestProjectileHit(MainCharacter, MuzzleStartLocation, OutHitResult.ImpactPoint);
                }
            }
            else
            {
                
                UE_LOG(LogFireProjectile, Warning, TEXT("---------------- I MISS"));
                //MainCharacter->HitValidTarget.Broadcast(false);
                if (DrawTraces)
                {
                    UPxiiDebugTraceBPLibrary::DrawDebugArrowSimple(this,
                        MuzzleStartLocation, EndLocation,
                        FLinearColor::Red, 3.f);
    
                    UPxiiDebugTraceBPLibrary::DrawDebugSphereSimple(this,
                           EndLocation,
                           7.f, FColor::Cyan, 3.f);
                }
                
                if (UPxiiCombatComponent* CombatComp = IPxiiCombatInterface::Execute_GetCombatComponent(MainCharacter))
                {
                    if (bUseSphereTrace)
                    {
                        FVector Start = MuzzleStartLocation;
                        FVector End = EndLocation;

                        float Distance = FVector::Dist(Start, OutHitResult.ImpactPoint);

                        // Scale height: 500 height per 1000 units
                        float ArcHeight = (Distance / 1000.f) * 250.f;

                        // Clamp if you don’t want absurdly tall arcs
                        ArcHeight = FMath::Clamp(ArcHeight, 200.f, 3000.f);
    
                        CombatComp->TriggerProjectileTraceArc(OutHitResult.ImpactNormal, EndLocation, ArcHeight);
                    }
                    else
                    {
                        CombatComp->TriggerProjectileTrace(OutHitResult.ImpactNormal, EndLocation);
                    }
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
        }
}
