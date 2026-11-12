// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/PxiiArmbladeData.h"
#include "Data/PxiiArmbladeEffectData.h"
#include "Data/PxiiYinYangData.h"
#include "Weapon/PxiiWeaponBase.h"
#include "PxiiWeaponMelee.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API APxiiWeaponMelee : public APxiiWeaponBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPxiiArmbladeEffectData> ArmbladeEffectData;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlaySlashSFX();

	void PlaySlashSFX_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ApplyCost();
	void ApplyCost_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlaySlashImpact(EPhysicalSurface SurfaceType, const FVector& impactLocation);

	void PlaySlashImpact_Implementation(EPhysicalSurface SurfaceType, const FVector& impactLocation);

	FImpactSoundEffect GetImpactSoundEffect(EPhysicalSurface SurfaceType);

	UFUNCTION(BlueprintPure)
	FHitStopData GetHitStopData(EHitImpact hitImpact);

	UFUNCTION(BlueprintPure)
	UCameraShakeAsset* GetCameraShakeImpact(EHitImpact hitImpact);

	UFUNCTION(BlueprintPure)
	float GetArmbladeDamage(float bonusDamage, float armbladeMult);

private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UResourceGenerationCost> Cost;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UPxiiArmbladeData> ArmbladeData;
};
