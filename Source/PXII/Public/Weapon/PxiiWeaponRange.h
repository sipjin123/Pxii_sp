// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Data/PxiiRecoilPattern.h"
#include "Data/PxiiWeaponSfxData.h"
#include "Weapon/PxiiWeaponBase.h"
#include "PxiiWeaponRange.generated.h"

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
	void PlayMuzzleFlash();

	virtual void PlayMuzzleFlash_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayWeaponSfx();

	virtual void PlayWeaponSfx_Implementation();

	UPxiiRecoilPattern* GetAdsRecoil();
	
	UPxiiRecoilPattern* GetHipRecoil();
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPxiiRecoilPattern> AdsRecoil;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPxiiRecoilPattern> HipRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPxiiWeaponSfxData> WeaponSfx;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|VFX")
	UNiagaraComponent* MuzzleFlashComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> MuzzleFlashEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|VFX", meta = (AllowPrivateAccess = "true"))
	FName MuzzleSocketName = "Muzzle";

	FVector MuzzleLocation;

	bool HasMuzzleFlash = false;
};
