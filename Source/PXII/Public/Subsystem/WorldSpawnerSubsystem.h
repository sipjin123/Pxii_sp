// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WorldSpawnerSubsystem.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSpawnDamageText, FVector, Location, int32, Magnitude, bool, bCritical);
UCLASS(Abstract, Blueprintable)
class PXII_API UWorldSpawnerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;

public:
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnSpawnDamageText OnSpawnDamageText;
};
