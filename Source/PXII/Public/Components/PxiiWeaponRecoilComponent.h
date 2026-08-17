#pragma once
#include "Components/ActorComponent.h"
#include "Data/PxiiRecoilPattern.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Weapon/PxiiWeaponRange.h"

#include "PxiiWeaponRecoilComponent.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiWeaponRecoilComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	
	UPxiiWeaponRecoilComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitializeRecoilComp(APxiiWeaponRange* weaponMesh);

	void InitializeRecoilComp_Implementation(APxiiWeaponRange* weaponMesh);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FireShot(APlayerController* controller, USkeletalMeshComponent* weaponMesh, bool isAdsActive);

	void FireShot_Implementation(APlayerController* controller, USkeletalMeshComponent* weaponMesh, bool isAdsActive);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ResetRecoil();

	void ResetRecoil_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ResetShotIndex();

	void ResetShotIndex_Implementation();

	void ApplyCameraRecoil(float DeltaTime);
	
	void ApplyWeaponMeshRecoil(float DeltaTime);

	void PlayAnimationRecoil();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ToggleAdsActive(bool isAdsActive);
	
	void ToggleAdsActive_Implementation(bool isAdsActive);

	UPxiiRecoilPattern* GetRecoilPattern();
	
private:
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPxiiRecoilPattern> AdsRecoil;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPxiiRecoilPattern> HipfireRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float ShotResetTime = 0.3f;
	
	TWeakObjectPtr<APlayerController> CachedPC;
	TWeakObjectPtr<USkeletalMeshComponent> CachedWeaponMesh;
	TWeakObjectPtr<UPxiiRecoilPattern> Recoil;
	
	FVector WeaponMeshRestLocation;
	FRotator WeaponMeshRestRotation;

	FVector2D PreviousAppliedCameraKick = FVector2D::ZeroVector;
	FVector2D CurrentCameraKick = FVector2D::ZeroVector;
	FVector2D TargetCameraKick = FVector2D::ZeroVector;
	
	FVector CurrentWeaponOffset = FVector::ZeroVector;
	FRotator CurrentWeaponRotation = FRotator::ZeroRotator;
	
	int32 CurrentShotIndex = 0;
	FTimerHandle TimerHandle;
	bool AdsActive = false;

	float LastTimeShot = 0.0f;
};
