#pragma once
#include "Item/BaseItem.h"
#include "Item/BaseItemData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PxiiInventorySubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, UBaseItemData*, itemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemove, FGuid, instanceId);

UCLASS(BlueprintType)
class PXII_API UPxiiInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void Initialize();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LoadItemInventory(const FInventorySaveData& inventoryData);
	void LoadItemInventory_Implementation(const FInventorySaveData& inventoryData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FillSaveData(FInventorySaveData& inventoryData);
	void FillSaveData_Implementation(FInventorySaveData& inventoryData);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddItemToInventory(UBaseItemData* inItemData);
	void AddItemToInventory_Implementation(UBaseItemData* inItemData);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemoveItemToInventory(FGuid instanceId);
	void RemoveItemToInventory_Implementation(FGuid instanceId);

	bool TryAddItemStack(UBaseItemData* InItemData);

	UFUNCTION(BlueprintPure)
	TArray<UBaseItem*> GetAllItemWithAssetId(FPrimaryAssetId assetId);
	
	UFUNCTION(BlueprintPure)
	bool GetAllItems(TArray<UBaseItem*>& OutResult);

private:

	UPROPERTY()
	TMap<FGuid, UBaseItem*> ItemData;
	
};
