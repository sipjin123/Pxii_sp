// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/PxiiProjectileBase.h"
#include "PxiiChargedProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API APxiiChargedProjectile : public APxiiProjectileBase
{
	GENERATED_BODY()

public:
	APxiiChargedProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	float ChargeMagnitude = 0.f;
};
