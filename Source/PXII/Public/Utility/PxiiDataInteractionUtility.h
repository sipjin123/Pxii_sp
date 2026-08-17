// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"
#include "Settings/PxiiGameUserSettings.h"

/**
 * 
 */
class PXII_API FPxiiDataInteractionUtility
{
public:
	FPxiiDataInteractionUtility(const FString& InFuncPath);
	
	FString GetSavedValueAsString(FName InSettingsID) const;
	void SetValueToSaveFromString(FName InSettingsID, const FString& InValue);
	
private:
	FCachedPropertyPath CachedFuncPath;
	
	TWeakObjectPtr<UPxiiGameUserSettings> CachedGameSettings;
};
