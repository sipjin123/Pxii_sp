// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Combat/PxiiProjectileBase.h"
#include "GameplayTagContainer.h"
#include "CombatDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Combat Developer Settings"))
class PXII_API UCombatDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Projectiles | Projectile Classes Soft Reference", meta = (ForceInlineRow, Categories = "Pxii.Projectiles"))
	TMap<FGameplayTag, TSoftClassPtr<APxiiProjectileBase>> ProjectileClassesMap;
};
