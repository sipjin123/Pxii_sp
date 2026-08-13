// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/Object.h"
#include "PxiiDamageableInterface.generated.h"

/**
 * 
 */

UINTERFACE(MinimalAPI, BlueprintType)
class UPxiiDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

class PXII_API IPxiiDamageableInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	APawn* GetPawnOwner();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	void ApplyDamage(AActor* Source, float Magnitude, int32 Payload);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	EDamagePart GetPartType();
};