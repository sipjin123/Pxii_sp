// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Class.h"
#include "PxiiDamageType.generated.h"

UENUM(BlueprintType)
enum class EFactionType:uint8
{
	Neutral=0 UMETA(DisplayName="None"),
	Player=1 UMETA(DisplayName="Player"),
	Enemy=2 UMETA(DisplayName="Enemy"),
	Ally=3 UMETA(DisplayName="Ally"),
};

UENUM(BlueprintType)
enum class EDamageType:uint8
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

UENUM(BlueprintType)
enum class EDamagePart:uint8
{
	None=0 UMETA(DisplayName="None"),

	// Physical
	Head=1 UMETA(DisplayName="Head"),
	Weakness=2 UMETA(DisplayName="Weakness"),
	Body=3 UMETA(DisplayName="Body"),
	Resistance=4 UMETA(DisplayName="Resistance"),
	Armor=5 UMETA(DisplayName="Armor"),
	Invulnerable=6 UMETA(DisplayName="Invulnerable"),
};

UENUM(BlueprintType)
enum class EHitEffectType : uint8 {
	Default = 0			UMETA(DisplayName = "Default"),
	Flinch = 1		UMETA(DisplayName = "Flinch"),
	Knockback = 2		UMETA(DisplayName = "Knockback"),
	Knockdown = 3	UMETA(DisplayName = "Knockdown"),
	Knockbackdown = 4	UMETA(DisplayName = "Knockbackdown"),
	Stagger = 5	UMETA(DisplayName = "Stagger"),
	Stun = 6	UMETA(DisplayName = "Stun"),
	Slow = 7	UMETA(DisplayName = "Slow"),
	Freeze = 8	UMETA(DisplayName = "Freeze"),
	Timestop = 9	UMETA(DisplayName = "Timestop"),
	FlyAway = 10	UMETA(DisplayName = "FlyAway"),
	StandingUp = 11	UMETA(DisplayName = "StandingUp"),
	Ministun = 12	UMETA(DisplayName = "Ministun"),
};

UENUM(BlueprintType)
enum class ESpecialAction:uint8
{
	None=0 UMETA(DisplayName="None"),

	// Physical
	Parry=1 UMETA(DisplayName="Parry"),
	PerfectDodge=2 UMETA(DisplayName="PerfectDodge"),
	Magic=3 UMETA(DisplayName="Magic"),
};


UENUM(BlueprintType)
enum class EPxiiDirection:uint8
{
	None=0 UMETA(DisplayName="None"),

	// Physical
	Right=1 UMETA(DisplayName="Right"),
	Left=2 UMETA(DisplayName="Left"),
	Back=3 UMETA(DisplayName="Back"),
	Front=4 UMETA(DisplayName="Front"),
};
