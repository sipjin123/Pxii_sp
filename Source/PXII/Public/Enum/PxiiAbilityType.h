// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PxiiAbilityType.generated.h"

UENUM(BlueprintType)
enum class EMantraType:uint8
{
	Default=0 UMETA(DisplayName="Default"),
	Bloodthirst=1 UMETA(DisplayName="Bloodthirst"),
	Wind=2 UMETA(DisplayName="Wind"),
	Static=3 UMETA(DisplayName="Static"),
	Overwhelm=4 UMETA(DisplayName="Overwhelm"),
};

UENUM(BlueprintType)
enum class EMantraSlotType:uint8
{
	Equipment=0 UMETA(DisplayName="Equipment"),
	Weapon=1 UMETA(DisplayName="Weapon"),
	Blessing=2 UMETA(DisplayName="Blessing"),
	Hallow=3 UMETA(DisplayName="Hallow"),
};
