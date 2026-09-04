#pragma once
#include "BaseItemData.h"
#include "Engine/DataAsset.h"
#include "ItemDataMap.generated.h"

UCLASS(BlueprintType)
class PXII_API UBaseItemDataCollection : public UPrimaryDataAsset
{
	GENERATED_BODY()

};


UCLASS(BlueprintType)
class PXII_API UItemDataCollection : public UBaseItemDataCollection
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UBaseItemData>> Data;
};

UCLASS(BlueprintType)
class PXII_API UItemDataMap : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UBaseItemDataCollection>> ItemMap;
};