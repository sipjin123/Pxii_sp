// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Enum/PxiiDamageType.h"
#include "PxiiCombatPayloads.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDamageNotifPayload
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	AActor* TargetActor;

	UPROPERTY(BlueprintReadWrite)
	float DamageMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	bool bIsCritical = false;

	UPROPERTY(BlueprintReadWrite)
	EHitEffectType HitType;
};

UCLASS()
class PXII_API UPxiiCombatPayloads : public UObject
{
	GENERATED_BODY()
	
};
