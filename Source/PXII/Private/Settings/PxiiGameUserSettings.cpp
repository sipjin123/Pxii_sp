// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/PxiiGameUserSettings.h"
#include "Engine/Engine.h"
#include "Utility/PXIILogUtility.h"

UPxiiGameUserSettings* UPxiiGameUserSettings::Get()
{
	if (GEngine)
	{
		UPxiiGameUserSettings* GameUserSettings = Cast<UPxiiGameUserSettings>(GEngine->GetGameUserSettings());
		
		if (!IsValid(GameUserSettings))
		{
			PXII_LOG(ELogCategory::UI, Warning, TEXT("[% s]: Failed to cast to PxiiGameUserSettings"), *ThisClass::StaticClass()->GetName());
			return nullptr;
		}
		
		return GameUserSettings;
	}
	
	return nullptr;
}

void UPxiiGameUserSettings::Set(FName InCurrentSettingsID)
{
	const UEnum* EnumPtr = StaticEnum<EPxiiGameUserSettingsID>();
	
	if (!EnumPtr)
	{
		return;
	}
	
	CurrentSettingsID = static_cast<EPxiiGameUserSettingsID>(EnumPtr->GetValueByName(InCurrentSettingsID));
}

FString UPxiiGameUserSettings::GetCurrentGameSettings() const
{
	switch (CurrentSettingsID)
	{
		case EPxiiGameUserSettingsID::GameDifficulty:
			return CurrentGameDifficulty;
		case EPxiiGameUserSettingsID::Overall:
		case EPxiiGameUserSettingsID::Speech:
		case EPxiiGameUserSettingsID::Text:
		default:
			return FString();
	}
}

void UPxiiGameUserSettings::SetCurrentGameSettings(FString InSettingsValue)
{
	switch (CurrentSettingsID)
	{
	case EPxiiGameUserSettingsID::GameDifficulty:
		CurrentGameDifficulty = InSettingsValue;
		break;
	case EPxiiGameUserSettingsID::Overall:
	case EPxiiGameUserSettingsID::Speech:
	case EPxiiGameUserSettingsID::Text:
	default:
		break;
	}
}
