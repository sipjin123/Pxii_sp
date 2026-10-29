// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EPropsType:uint8
{
	Default=0 UMETA(DisplayName="Default"),
};


class PXII_API PxiiPropsData
{
public:
	PxiiPropsData();
	~PxiiPropsData();
};
