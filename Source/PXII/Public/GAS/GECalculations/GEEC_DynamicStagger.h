// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_DynamicStagger.generated.h"

/**
 * 
 */
DECLARE_LOG_CATEGORY_EXTERN(LogGEECStagger, Log, All);

UCLASS()
class PXII_API UGEEC_DynamicStagger : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UGEEC_DynamicStagger();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
