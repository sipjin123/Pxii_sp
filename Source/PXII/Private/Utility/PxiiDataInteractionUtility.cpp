// Fill out your copyright notice in the Description page of Project Settings.

#include "Utility/PxiiDataInteractionUtility.h"


FPxiiDataInteractionUtility::FPxiiDataInteractionUtility(const FString& InFuncPath) : CachedFuncPath(InFuncPath)
{
	if (!CachedGameSettings.IsValid())
	{
		CachedGameSettings = UPxiiGameUserSettings::Get();
	}
}

FString FPxiiDataInteractionUtility::GetSavedValueAsString(FName InSettingsID) const
{
	CachedGameSettings->Set(InSettingsID);
	
	FString Value;
	
	PropertyPathHelpers::GetPropertyValueAsString(CachedGameSettings.Get(), CachedFuncPath, Value);
	
	return Value;
}

void FPxiiDataInteractionUtility::SetValueToSaveFromString(FName InSettingsID, const FString& InValue)
{
	CachedGameSettings->Set(InSettingsID);
	
	PropertyPathHelpers::SetPropertyValueFromString(CachedGameSettings.Get(), CachedFuncPath, InValue);
}
