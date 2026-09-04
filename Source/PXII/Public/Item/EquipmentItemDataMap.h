#pragma once
#include "EquipmentSlot.h"
#include "ItemDataMap.h"
#include "Engine/DataAsset.h"
#include "EquipmentItemDataMap.generated.h"

UCLASS(BlueprintType)
class PXII_API UEquipmentItemDataCollection : public UBaseItemDataCollection
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UEquipmentItemData>> Data;
};

USTRUCT(BlueprintType)
struct FEquipmentSlotMap
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipmentSlot SlotType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UEquipmentItemDataCollection> Collection;
};

UCLASS(BlueprintType)
class PXII_API UEquipmentItemDataMap : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FEquipmentSlotMap> Map;
};
