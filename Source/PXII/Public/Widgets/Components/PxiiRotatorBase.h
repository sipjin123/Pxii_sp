// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "PxiiRotatorBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class PXII_API UPxiiRotatorBase : public UCommonRotator
{
	GENERATED_BODY()
	
public:
	void SetSelectedOptionByText(const FText& InText);
	
	UFUNCTION(BlueprintPure, Category = "Rotator")
	FORCEINLINE TArray<FText> GetAllOptions() { return TextLabels; }
};
