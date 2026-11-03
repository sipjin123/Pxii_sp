// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/PxiiArmbladeEffectData.h"
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

	void PlaySlashSFX();
	
};
