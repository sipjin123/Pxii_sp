// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/PxiiGameUserSettings.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Subsystem/PxiiUISubsystem.h"
#include "Utility/PXIILogUtility.h"
#include "Widgets/SWindow.h"

UPxiiGameUserSettings::UPxiiGameUserSettings()
	: OverallVolume(1.f)
	, MusicVolume(1.f)
{
	// Initialize default values
	
	WindowModePtr = StaticEnum<EWindowMode::Type>();
}

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
		case EPxiiGameUserSettingsID::KeyboardAndMouse:
			return CurrentKeyboardAndMouseMap;
		case EPxiiGameUserSettingsID::Gamepad:
			return CurrentGamepadMap;
		case EPxiiGameUserSettingsID::InvertMouseLook:
		case EPxiiGameUserSettingsID::Display:
			return CurrentDisplayID;
		case EPxiiGameUserSettingsID::DisplayMode:
			return WindowModePtr->GetNameStringByValue(CurrentDisplayMode);
		case EPxiiGameUserSettingsID::DisplayResolution:
			return CurrentDisplayResolution.ToString();
		case EPxiiGameUserSettingsID::FramerateCap:
		case EPxiiGameUserSettingsID::Overall:
			return LexToString(OverallVolume);
		case EPxiiGameUserSettingsID::Music:
			return LexToString(MusicVolume);
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
	case EPxiiGameUserSettingsID::KeyboardAndMouse:
		CurrentKeyboardAndMouseMap = InSettingsValue;
		break;
	case EPxiiGameUserSettingsID::Gamepad:
		CurrentGamepadMap = InSettingsValue;
		break;
	case EPxiiGameUserSettingsID::InvertMouseLook:
	case EPxiiGameUserSettingsID::Display:
		CurrentDisplayID = InSettingsValue;
		
		SetActiveMonitorID(InSettingsValue);
		break;
	case EPxiiGameUserSettingsID::DisplayMode:
		CurrentDisplayMode = static_cast<EWindowMode::Type>(WindowModePtr->GetValueByName(*InSettingsValue));
		
		SetFullscreenMode(CurrentDisplayMode);
		//ApplyResolutionSettings(false);
		break;
	case EPxiiGameUserSettingsID::DisplayResolution:
		CurrentDisplayResolution.FIntPoint::InitFromString(InSettingsValue);
		
		SetScreenResolution(CurrentDisplayResolution);
		//ApplyResolutionSettings(false);
		break;
	case EPxiiGameUserSettingsID::FramerateCap:
	case EPxiiGameUserSettingsID::Overall:
		LexFromString(OverallVolume, *InSettingsValue);
		// call logic for control volume
		break;
	case EPxiiGameUserSettingsID::Music:
		LexFromString(MusicVolume, *InSettingsValue);
		// call logic for control volume
		break;
	case EPxiiGameUserSettingsID::Speech:
	case EPxiiGameUserSettingsID::Text:
	default:
		break;
	}
}

TArray<FMonitorInfo> UPxiiGameUserSettings::GetAllDisplayMonitorInfo()
{
	FDisplayMetrics DisplayMetrics;
	FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);
	return DisplayMetrics.MonitorInfo;
}

void UPxiiGameUserSettings::SetActiveMonitorID(const FString& InDisplayID)
{
	FDisplayMetrics DisplayMetrics;
	FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);
	for (const FMonitorInfo& MonitorInfo : DisplayMetrics.MonitorInfo)
	{
		if (MonitorInfo.ID == InDisplayID)
		{
			PXII_LOG(ELogCategory::UI, Warning, TEXT("[% s]: Display changed: %s"), *ThisClass::StaticClass()->GetName(), *InDisplayID);
			
			FVector2D NewWindowPosition(MonitorInfo.WorkArea.Left, MonitorInfo.WorkArea.Top);
			
			if (GEngine && GEngine->GameViewport)
			{
				TSharedPtr<SWindow> GameWindow = GEngine->GameViewport->GetWindow();
				
				if (GameWindow.IsValid())
				{
					GameWindow->MoveWindowTo(NewWindowPosition);
				}
				else
				{
					PXII_LOG(ELogCategory::UI, Warning, TEXT("[% s]: Invalid game viewport"), *ThisClass::StaticClass()->GetName());
				}
			}
			
			return;
		}
	}
}

TArray<FIntPoint> UPxiiGameUserSettings::GetSupportedFullscreenResolutions()
{
	TArray<FIntPoint> Resolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);
	return Resolutions;
}
