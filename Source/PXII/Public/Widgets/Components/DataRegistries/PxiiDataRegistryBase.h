// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectCollection.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectBase.h"
#include "PxiiDataRegistryBase.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiDataRegistryBase : public UObject
{
	GENERATED_BODY()
	
public:
	// Called by the screens after the creation of object type UDataRegistry
	virtual void InitDataRegistry(ULocalPlayer* InOwningLocalPlayer);

private:
	// Create respective tab initialization function in your child based on your need
	// Then call these in InitDataRegistry
};
