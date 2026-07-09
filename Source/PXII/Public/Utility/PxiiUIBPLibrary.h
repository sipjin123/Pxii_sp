// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "CommonUI/PxiiActivatableWidget.h"
#include "PxiiUIBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiUIBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "UI | BP Function Library")
	static TSoftClassPtr<UPxiiActivatableWidget> GetWidgetSoftClassByTag(UPARAM(meta = (Categories = "Pxii.UI.Widget")) const FGameplayTag InWidgetTag);
};
