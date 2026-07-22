// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MoverComponent.h"
#include "PxiiGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	UPxiiGameplayAbilityBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure)
	UMoverComponent* GetMoverComponent();
};
