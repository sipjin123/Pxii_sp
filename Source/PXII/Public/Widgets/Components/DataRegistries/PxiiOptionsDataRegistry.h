// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PxiiDataRegistryBase.h"
#include "PxiiOptionsDataRegistry.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiOptionsDataRegistry : public UPxiiDataRegistryBase
{
	GENERATED_BODY()
	
public:
	// ~Begin UPxiiDataRegistryBase
	virtual void InitDataRegistry(ULocalPlayer* InOwningLocalPlayer) override;
	// ~End UPxiiDataRegistryBase

	FORCEINLINE const TArray<UPxiiListDataObjectCollection*>& GetRegisteredTabsCollections() const { return RegisteredTabsCollections; }
	FORCEINLINE void SetOptionsData(UPxiiOptionsData* InOptionsData) { OptionsData = InOptionsData; };

	TArray<UPxiiListDataObjectBase*> GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID);

private:
	void ConstructTabs();
	void ConstructDataObjects(UPxiiListDataObjectCollection* CollectionToAdd, const TArray<FOptionsDataEntry>& DataEntries);
	void FindChildListDataRecursively(UPxiiListDataObjectBase* InParentData, TArray<UPxiiListDataObjectBase*>& OutFoundChildListData) const;

	UPROPERTY(Transient)
	TArray<UPxiiListDataObjectCollection*> RegisteredTabsCollections;
	
	UPROPERTY()
	UPxiiOptionsData* OptionsData;
};
