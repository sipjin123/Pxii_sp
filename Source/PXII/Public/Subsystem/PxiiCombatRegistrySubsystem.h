// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PxiiCombatRegistrySubsystem.generated.h"

/**
 * 
 */

DECLARE_LOG_CATEGORY_EXTERN(LogCombatRegistrySubsystem, Log, All);
UCLASS(Abstract, Blueprintable)
class PXII_API UPxiiCombatRegistrySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
};
