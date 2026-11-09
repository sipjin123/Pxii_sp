// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Data/PxiiBlasterData.h"
#include "Data/PxiiRecoilPattern.h"
#include "Data/PxiiWeaponSfxData.h"
#include "Weapon/PxiiWeaponBase.h"
#include "PxiiWeaponRange.generated.h"

USTRUCT(BlueprintType)
struct FMuzzleEffect
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|VFX")
	TArray<TObjectPtr<UNiagaraSystem>> MuzzleEffect;
};

USTRUCT(BlueprintType)
struct FMuzzleEffectComp
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|VFX")
	TArray<TObjectPtr<UNiagaraComponent>> MuzzleEffectComp;
};

/**
 * 
 */
UCLASS()
class PXII_API APxiiWeaponRange : public APxiiWeaponBase
{
	GENERATED_BODY()

public:
	
	APxiiWeaponRange();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayMuzzleFlash(FName MuzzleName = "Default");

	virtual void PlayMuzzleFlash_Implementation(FName MuzzleName = "Default");

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopMuzzleFlash(FName MuzzleName = "Default");

	virtual void StopMuzzleFlash_Implementation(FName MuzzleName = "Default");

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayWeaponSfx();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ResetVent();

	void ResetVent_Implementation();

	UFUNCTION(BlueprintPure)
	float GetDamage(float hitDistance, float bulletBonusDamage = 0.0f, bool isCritical = false);

	virtual void PlayWeaponSfx_Implementation();

	UPxiiRecoilPattern* GetAdsRecoil();
	
	UPxiiRecoilPattern* GetHipRecoil();

	// TArray<UNiagaraSystem*> GetSystem();

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPxiiBlasterData> BlasterData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPxiiRecoilPattern> AdsRecoil;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPxiiRecoilPattern> HipRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPxiiWeaponSfxData> WeaponSfx;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|VFX")
	TMap<FName, FMuzzleEffectComp> MuzzleFlashComponents;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|VFX", meta = (AllowPrivateAccess = "true"))
	TMap<FName, FMuzzleEffect> MuzzleFlashEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|VFX", meta = (AllowPrivateAccess = "true"))
	FName MuzzleSocketName = "Muzzle";

	FVector MuzzleLocation;

	bool HasMuzzleFlash = false;
};
