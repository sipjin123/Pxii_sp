// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_DynamicDamage.generated.h"

/**
 * 
 */

DECLARE_LOG_CATEGORY_EXTERN(LogGEECDamage, Log, All);
UCLASS()
class PXII_API UGEEC_DynamicDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGEEC_DynamicDamage();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
