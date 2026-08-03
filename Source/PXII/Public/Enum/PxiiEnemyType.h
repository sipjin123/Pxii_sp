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
	Small=1 UMETA(DisplayName="Small"),
	Medium=2 UMETA(DisplayName="Medium"),
	Large=3 UMETA(DisplayName="Large"),
};

UENUM(BlueprintType)
enum class ENPCState:uint8
{
	Default=0 UMETA(DisplayName="Default"),
	Attacking=1 UMETA(DisplayName="Attacking"),
	Reposition=2 UMETA(DisplayName="Reposition"),
	Patrol=3 UMETA(DisplayName="Patrol"),
	Retreat=4 UMETA(DisplayName="Retreat"),
	PosToAttack=5 UMETA(DisplayName="PosToAttack"),
	Disabled=6 UMETA(DisplayName="Disabled"),
	Wait=7 UMETA(DisplayName="Wait"),
	SearchingTarget=8 UMETA(DisplayName="SearchingTarget"),
	Assessment=9 UMETA(DisplayName="Assessment"),
};