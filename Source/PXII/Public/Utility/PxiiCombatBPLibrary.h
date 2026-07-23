// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Combat/PxiiProjectileBase.h"
#include "GameplayTagContainer.h"
#include "Character/PxiiCharacter.h"
#include "PxiiCombatBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiCombatBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Combat | BP Function Library")
	static TSoftClassPtr<APxiiProjectileBase> GetSoftProjectileClassByTag(UPARAM(meta = (Categories = "Pxii.Projectiles")) FGameplayTag InTag);

	UFUNCTION(BlueprintCallable)
	static void StartProjectileTrace(APxiiCharacter* character, FName MuzzleSocketName = "Muzzle");
};
