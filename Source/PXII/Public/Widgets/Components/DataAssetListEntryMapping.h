// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ListEntries/PxiiListEntryBase.h"
#include "DataObject/PxiiListDataObjectBase.h"
#include "DataAssetListEntryMapping.generated.h"

/**
 *
 */
UCLASS()
class PXII_API UDataAssetListEntryMapping : public UDataAsset
{
	GENERATED_BODY()

public: 
	TSubclassOf<UPxiiListEntryBase> FindListEntryClassByDataObject(UPxiiListDataObjectBase* InDataObject) const;

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UPxiiListDataObjectBase>, TSubclassOf<UPxiiListEntryBase>> DataObjectListEntryMap;
};