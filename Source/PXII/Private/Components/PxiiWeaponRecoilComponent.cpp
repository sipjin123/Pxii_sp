#include "Components/PxiiWeaponRecoilComponent.h"

#include "Animation/AnimInstance.h"

UPxiiWeaponRecoilComponent::UPxiiWeaponRecoilComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UPxiiWeaponRecoilComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LastTimeShot += DeltaTime;
	
	ApplyCameraRecoil(DeltaTime);
	ApplyWeaponMeshRecoil(DeltaTime);
	
}

void UPxiiWeaponRecoilComponent::InitializeRecoilComp_Implementation(APxiiWeaponRange* WeaponRange)
{
	AdsRecoil = WeaponRange->GetAdsRecoil();
	HipfireRecoil = WeaponRange->GetHipRecoil();
}

void UPxiiWeaponRecoilComponent::FireShot_Implementation(APlayerController* controller, USkeletalMeshComponent* weaponMesh, bool isAdsActive)
{
	if(!AdsRecoil || !HipfireRecoil)
	{
		return;
	}

	if(!controller || !weaponMesh)
	{
		return;
	}


	
	CachedPC = controller;
	CachedWeaponMesh = weaponMesh;

	if (WeaponMeshRestLocation.IsZero())
	{
		WeaponMeshRestLocation = weaponMesh->GetRelativeLocation();
		WeaponMeshRestRotation = weaponMesh->GetRelativeRotation();
	}
	
	ToggleAdsActive(isAdsActive);
	Recoil = GetRecoilPattern();
	
	int32 PatternIndex = FMath::Min(CurrentShotIndex, Recoil->KickPattern.Num() - 1);
	FVector2D BaseKick = Recoil->KickPattern.IsValidIndex(PatternIndex)
		? Recoil->KickPattern[PatternIndex]
		: FVector2D(0.f, 1.0f);
	
	float RandomH = FMath::RandRange(-Recoil->HorizontalRandomness, Recoil->HorizontalRandomness);

	TargetCameraKick += FVector2D(
		(BaseKick.X + RandomH) * Recoil->PatternScale,
		BaseKick.Y * Recoil->PatternScale
	);

	CurrentWeaponOffset += Recoil->WeaponKickOffset;
	CurrentWeaponRotation += Recoil->WeaponKickRotation;

	PlayAnimationRecoil();

	CurrentShotIndex++;
	
	UWorld* world = GetWorld();
	if(!world)
	{
		return;
	}
	
	LastTimeShot = 0.0f;

	world->GetTimerManager().ClearTimer(TimerHandle);
	world->GetTimerManager().SetTimer(TimerHandle, this, &UPxiiWeaponRecoilComponent::ResetShotIndex,
		ShotResetTime, false);
}

void UPxiiWeaponRecoilComponent::ResetRecoil_Implementation()
{
	ResetShotIndex();
	CurrentCameraKick = FVector2D::ZeroVector;
	TargetCameraKick = FVector2D::ZeroVector;
	CurrentWeaponOffset = FVector::ZeroVector;
	CurrentWeaponRotation = FRotator::ZeroRotator;
}

void UPxiiWeaponRecoilComponent::ResetShotIndex_Implementation()
{
	CurrentShotIndex = 0;
}

void UPxiiWeaponRecoilComponent::ApplyCameraRecoil(float DeltaTime)
{
	if (!CachedPC.IsValid() || !Recoil.Get())
	{
		return;	
	}

	CurrentCameraKick = FMath::Vector2DInterpTo(CurrentCameraKick, TargetCameraKick, DeltaTime, Recoil->CameraKickSpeed);

	if (LastTimeShot > Recoil->CameraRecoveryDelay)
	{
		TargetCameraKick = FMath::Vector2DInterpTo(TargetCameraKick, FVector2D::ZeroVector, DeltaTime,
			Recoil->CameraRecoverySpeed);
	}

	FVector2D FrameDelta = CurrentCameraKick - PreviousAppliedCameraKick;

	APawn* Pawn = CachedPC->GetPawn();
	if (Pawn)
	{
		FRotator NewControlRotation = CachedPC->GetControlRotation();
		NewControlRotation.Pitch -= FrameDelta.Y; 
		NewControlRotation.Yaw += FrameDelta.X;
		CachedPC->SetControlRotation(NewControlRotation);
	}

	PreviousAppliedCameraKick = CurrentCameraKick;
}

void UPxiiWeaponRecoilComponent::ApplyWeaponMeshRecoil(float DeltaTime)
{
	if (!CachedWeaponMesh.IsValid() || !Recoil.Get())
	{
		return;	
	}

	CurrentWeaponOffset = FMath::VInterpTo(
		CurrentWeaponOffset, FVector::ZeroVector, DeltaTime, Recoil->WeaponRecoverySpeed);
	CurrentWeaponRotation = FMath::RInterpTo(
		CurrentWeaponRotation, FRotator::ZeroRotator, DeltaTime, Recoil->WeaponRecoverySpeed);

	CachedWeaponMesh->SetRelativeLocation(WeaponMeshRestLocation + CurrentWeaponOffset);
	CachedWeaponMesh->SetRelativeRotation(WeaponMeshRestRotation + CurrentWeaponRotation);
}

void UPxiiWeaponRecoilComponent::PlayAnimationRecoil()
{
	UPxiiRecoilPattern* recoil = GetRecoilPattern();
	if (!CachedWeaponMesh.IsValid() || !recoil || !recoil->RecoilAnimMontage)
	{
		return;
	}

	if (UAnimInstance* AnimInst = CachedWeaponMesh->GetAnimInstance())
	{
		AnimInst->Montage_Play(recoil->RecoilAnimMontage, recoil->AnimRecoilPlayRate);
	}
}

void UPxiiWeaponRecoilComponent::ToggleAdsActive_Implementation(bool isAdsActive)
{
	AdsActive = isAdsActive;	
}

UPxiiRecoilPattern* UPxiiWeaponRecoilComponent::GetRecoilPattern()
{
	if(!HipfireRecoil || !AdsRecoil)
	{
		return nullptr;
	}
	
	return AdsActive ? AdsRecoil : HipfireRecoil;
}
