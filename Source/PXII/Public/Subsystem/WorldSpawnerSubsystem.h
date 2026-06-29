// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldSpawnerSubsystem.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class PXII_API UWorldSpawnerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
};
