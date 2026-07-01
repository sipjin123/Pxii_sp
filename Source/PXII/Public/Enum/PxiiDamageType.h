// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "PxiiDamageType.generated.h"

UENUM(BlueprintType)
enum class EPxiiDamageType:uint8
{
	None=0 UMETA(DisplayName="None"),

	// Physical
	Physical=1 UMETA(DisplayName="Physical"),
	Slash=2 UMETA(DisplayName="Slash"),
	Pierce=3 UMETA(DisplayName="Pierce"),
	Blunt=4 UMETA(DisplayName="Blunt"),

	// Magic
	Fire=10 UMETA(DisplayName="Fire"),
	Ice=11 UMETA(DisplayName="Ice"),
	Lightning=12 UMETA(DisplayName="Lightning"),
	Poison=13 UMETA(DisplayName="Poison"),
	Arcane=14 UMETA(DisplayName="Arcane"),

	// Special
	TrueDamage=20 UMETA(DisplayName="True Damage"),
	Fall=21 UMETA(DisplayName="Fall"),
	Environmental=22 UMETA(DisplayName="Environmental"),
	Execute=23 UMETA(DisplayName="Execute"),

	MAX=255 UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EDamageSource:uint8
{
	None=0 UMETA(DisplayName="None"),

	// Physical
	Melee=1 UMETA(DisplayName="Melee"),
	Range=2 UMETA(DisplayName="Range"),
	Magic=3 UMETA(DisplayName="Magic"),
};