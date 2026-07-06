// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PxiiEnemyType.generated.h"

UENUM(BlueprintType)
enum class EEnemyType:uint8
{
	None=0 UMETA(DisplayName="None"),
	Grunt=1 UMETA(DisplayName="Grunt"),
	Enforcers=2 UMETA(DisplayName="Enforcers"),
	Captains=3 UMETA(DisplayName="Captains"),
	Boss=4 UMETA(DisplayName="Boss"),
};

UENUM(BlueprintType)
enum class EBodySizeType:uint8
{
	Default=0 UMETA(DisplayName="Default"),
	Small=0 UMETA(DisplayName="Small"),
	Medium=0 UMETA(DisplayName="Medium"),
	Large=0 UMETA(DisplayName="Large"),
};