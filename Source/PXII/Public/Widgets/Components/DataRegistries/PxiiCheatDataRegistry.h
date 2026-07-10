// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Components/DataRegistries/PxiiDataRegistryBase.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectBase.h"
#include "PxiiCheatDataRegistry.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiCheatDataRegistry : public UPxiiDataRegistryBase
{
	GENERATED_BODY()
	
public:
	// ~Begin UPxiiDataRegistryBase
	virtual void InitDataRegistry(ULocalPlayer* InOwningLocalPlayer) override;
	// ~End UPxiiDataRegistryBase

	FORCEINLINE const TArray<UPxiiListDataObjectCollection*>& GetRegisteredTabsCollections() const { return RegisteredTabsCollections; }

	TArray<UPxiiListDataObjectBase*> GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID);

private:
	void InitCheatTab1();
	void InitCheatTab2();
	void InitCheatTab3();

	UPROPERTY(Transient)
	TArray<UPxiiListDataObjectCollection*> RegisteredTabsCollections;
};
