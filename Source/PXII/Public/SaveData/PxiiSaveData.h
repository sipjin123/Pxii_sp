#pragma once
#include "GameFramework/SaveGame.h"
#include "Item/EquipmentItemData.h"
#include "PxiiSaveData.generated.h"

USTRUCT(BlueprintType)
struct FCustomizationSaveData
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY()
	FString HeadSlotItemKey;

	UPROPERTY()
	FString UpperSlotItemKey;
	
	UPROPERTY()
	FString LowerSlotItemKey;

	UPROPERTY()
	FString TattooSlotKey;

	UPROPERTY()
	FString BlasterSlotKey;
};

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int32 Level = 1;

	UPROPERTY()
	int32 Experience = 0;

	UPROPERTY()
	int32 Gold = 0;
};

USTRUCT(BlueprintType)
struct FCheckpointData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FVector PlayerLocation;

	UPROPERTY()
	FRotator PlayerRotation;
};

USTRUCT(BlueprintType)
struct FItemSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FPrimaryAssetId AssetId;
	
	UPROPERTY()
	FGuid InstanceId;

	UPROPERTY()
	int32 Quantity;
};

USTRUCT(BlueprintType)
struct FEquipmentSlotSaveData : public FItemSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	int32 SlotIndex;
	
	UPROPERTY()
	bool isEquipped;
};

USTRUCT()
struct FEquipmentSlotSaveContainer
{
	GENERATED_BODY()

public:
	TArray<FEquipmentSlotSaveData> Slots;

	bool HasSlot(int32 slotIndex) const 
	{
		bool hasSlotIndex = Slots.ContainsByPredicate(
		[slotIndex](const FEquipmentSlotSaveData& data)
		{
			return data.SlotIndex == slotIndex;
		});
		
		return hasSlotIndex;
	}
	
	FEquipmentSlotSaveData GetSlot(int32 slotIndex) const
	{
		const int32 index = Slots.IndexOfByPredicate(
		[slotIndex](const FEquipmentSlotSaveData& data)
		{
			return 	data.SlotIndex == slotIndex;
		});

		if (index != INDEX_NONE)
		{
			return Slots[index];
		}

		return Slots[0];
	}
};

USTRUCT(BlueprintType)
struct FEquipmentSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<EEquipmentSlot, FEquipmentSlotSaveContainer> EquipmentSaveData;
};

USTRUCT(BlueprintType)
struct FInventorySaveData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FItemSaveData> Items;
};

UCLASS()
class PXII_API UPxiiSaveData : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FPlayerData Player;

	UPROPERTY()
	FCheckpointData Checkpoint;

	UPROPERTY()
	FCustomizationSaveData Customization;

	UPROPERTY()
	FEquipmentSaveData Equipment;

	UPROPERTY()
	FInventorySaveData Inventory;
};
