// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "CommonUI\PxiiActivatableWidget.h"
#include "UIDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "UI Developer Settings"))
class PXII_API UUIDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
};
