// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PxiiDebugTraceBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiDebugTraceBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category="Debug", meta=(WorldContext="WorldContextObject"))
	static void DrawDebugSphereSimple(UObject* WorldContextObject, FVector Location, float Radius, FLinearColor Color,
	                           float Duration);
};
