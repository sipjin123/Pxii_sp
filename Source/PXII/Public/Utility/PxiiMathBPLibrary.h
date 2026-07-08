// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PxiiMathBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiMathBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="PXII|Math")
	static FVector GetLocationInFrontOfActor(const AActor* Actor, float Distance);
};
