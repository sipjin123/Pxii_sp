// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/Object.h"
#include "PxiiInteractableInterface.generated.h"

/**
 * 
 */

UINTERFACE(MinimalAPI, BlueprintType)
class UPxiiInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class PXII_API IPxiiInteractableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	void Interact(APawn* Source);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	int32 GetPriority();
};