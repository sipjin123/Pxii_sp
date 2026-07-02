// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "PxiiGameplayEffectBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class PXII_API UPxiiGameplayEffectBase : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UPxiiGameplayEffectBase();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bShowFloatingText = true;
};
