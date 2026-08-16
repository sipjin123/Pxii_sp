// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "PxiiGameUserSettings.generated.h"

UENUM(BlueprintType)
enum class EPxiiGameUserSettingsID : uint8
{
#pragma region Gameplay
	GameDifficulty,
#pragma endregion
	
#pragma region Controls
	
#pragma endregion
	
#pragma region Graphics
	
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
	static UPxiiGameUserSettings* Get();
	void Set(FName InCurrentSettingsID);
	
#pragma region Collection Tab
	UFUNCTION()
	FString GetCurrentGameSettings() const;
	
	UFUNCTION()
	void SetCurrentGameSettings(FString InSettingsValue);
#pragma endregion 
	
private:
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	
	UPROPERTY()
	EPxiiGameUserSettingsID CurrentSettingsID { EPxiiGameUserSettingsID::None };
};
