// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PxiiInventoryType.generated.h"

UENUM(BlueprintType)
enum class EEquipmentType:uint8
{
	Head=0 UMETA(DisplayName="None"),
	Torso=1 UMETA(DisplayName="Torso"),
	Legs=2 UMETA(DisplayName="Legs"),
};

UENUM(BlueprintType)
enum class ERewardType:uint8
{
	Exp=0 UMETA(DisplayName="Exp"),
	Gold=1 UMETA(DisplayName="Gold"),
	Shard=2 UMETA(DisplayName="Shard"),
};

UENUM(BlueprintType)
enum class ERarityType:uint8
{
	Common=0 UMETA(DisplayName="Common"),
	Rare=1 UMETA(DisplayName="Rare"),
	Epic=2 UMETA(DisplayName="Epic"),
	Legendary=3 UMETA(DisplayName="Legendary"),
	LegendaryT1=4 UMETA(DisplayName="LegendaryT1"),
	LegendaryT2=5 UMETA(DisplayName="LegendaryT2"),
	LegendaryT3=6 UMETA(DisplayName="LegendaryT3"),
};
