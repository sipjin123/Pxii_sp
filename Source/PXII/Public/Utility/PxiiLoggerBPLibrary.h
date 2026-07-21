// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PXIILogUtility.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PxiiLoggerBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiLoggerBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category="PXII|Debug")
	static void PXIIPrintLog(ELogCategory LogCategory, EPXIILogVerbosity Verbosity, FString InString);

	UFUNCTION(BlueprintCallable, Category="PXII|Debug")
	static void EnableLogCategories(TArray<ELogCategory> Category);

	UFUNCTION(BlueprintCallable, Category="PXII|Debug")
	static void DisableLogCategories(TArray<ELogCategory> Category);

	UFUNCTION(BlueprintCallable, Category="PXII|Debug")
	static void EnableLogCategory(ELogCategory Category);

	UFUNCTION(BlueprintCallable, Category="PXII|Debug")
	static void DisableLogCategory(ELogCategory Category);
};
