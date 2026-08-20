// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include  "GenericPlatform/GenericApplication.h"
#include "PxiiGameUserSettings.generated.h"

UENUM(BlueprintType)
enum class EPxiiGameUserSettingsID : uint8
{
#pragma region Gameplay
	GameDifficulty,
#pragma endregion
	
#pragma region Controls
	KeyboardAndMouse,
	Gamepad,
	InvertMouseLook,
#pragma endregion
	
#pragma region Graphics
	Display,
	DisplayMode,
	DisplayResolution,
	FramerateCap,
#pragma endregion
	
#pragma region Audio
	Overall,
	Music,
#pragma endregion
	
#pragma region Language
	Text,
	Speech,
#pragma endregion
	
	//Condition check
	None,
};

/**
 * 
 */
UCLASS()
class PXII_API UPxiiGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	UPxiiGameUserSettings();
	
	static UPxiiGameUserSettings* Get();
	void Set(FName InCurrentSettingsID);
	
	UFUNCTION()
	FString GetCurrentGameSettings() const;
	
	UFUNCTION()
	void SetCurrentGameSettings(FString InSettingsValue);
	
	static TArray<FMonitorInfo> GetAllDisplayMonitorInfo();
	void SetActiveMonitorID(const FString& InDisplayID);
	
	static TArray<FIntPoint> GetSupportedFullscreenResolutions();
	
private:
	UPROPERTY()
	const UEnum* WindowModePtr;
	
#pragma region Gameplay Tab
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
#pragma endregion
	
#pragma region Controls Tab
	UPROPERTY(Config)
	FString CurrentKeyboardAndMouseMap;
	
	UPROPERTY(Config)
	FString CurrentGamepadMap;
#pragma endregion 
	
#pragma region Graphics
	UPROPERTY(Config)
	FString CurrentDisplayID;
	
	UPROPERTY(Config)
	TEnumAsByte<EWindowMode::Type> CurrentDisplayMode;
	
	UPROPERTY(Config)
	FIntPoint CurrentDisplayResolution;
#pragma endregion 
	
#pragma region Audio Tab
	UPROPERTY(Config)
	float OverallVolume;
	
	UPROPERTY(Config)
	float MusicVolume;
#pragma endregion 
	
#pragma region Language
	
#pragma endregion
	
	UPROPERTY()
	EPxiiGameUserSettingsID CurrentSettingsID { EPxiiGameUserSettingsID::None };
};
