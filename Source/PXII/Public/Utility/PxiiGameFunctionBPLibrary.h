// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utility/LatentAction/PxiiCountDownAction.h"
#include "PxiiGameFunctionBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiGameFunctionBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "PXII|CountDownAction", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput", TotalTime = "1.0", UpdateInterval = "0.1"))
	static void CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutTimeRemaining,
	ECountDownActionInput CountDownInput, UPARAM(DisplayName = "Output") ECountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo);
};
