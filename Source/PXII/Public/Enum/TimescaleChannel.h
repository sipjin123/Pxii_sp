// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "TimescaleChannel.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETimescaleChannel:uint8
{
	Global=0 UMETA(DisplayName="Global"),
	Player=1 UMETA(DisplayName="Player"),
	Enemy=2 UMETA(DisplayName="Enemy"),
	Projectile=3 UMETA(DisplayName="Projectile"),

};
