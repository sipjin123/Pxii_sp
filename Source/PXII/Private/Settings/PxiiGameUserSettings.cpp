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
