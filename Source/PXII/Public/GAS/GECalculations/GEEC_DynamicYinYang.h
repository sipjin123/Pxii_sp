// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_DynamicYinYang.generated.h"

/**
 * 
 */
DECLARE_LOG_CATEGORY_EXTERN(LogGEECYinYang, Log, All);
UCLASS()
class PXII_API UGEEC_DynamicYinYang : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UGEEC_DynamicYinYang();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
