#pragma once
#include "Components/ActorComponent.h"
#include "Item/BaseItem.h"
#include "Item/BaseItemData.h"
#include "Item/EquipmentItemData.h"
#include "SaveData/PxiiSaveData.h"
#include "Subsystem/PxiiInventorySubsystem.h"
#include "PxiiInventoryComponent.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	void FillSaveData(FInventorySaveData& saveData);
	
	UFUNCTION(BlueprintPure)
	void GetAllBlasterItemId(TArray<FPrimaryAssetId>& AssetIds);

	UFUNCTION(BlueprintPure)
	void GetAllArmbladeItemId(TArray<FPrimaryAssetId>& AssetIds);

	UFUNCTION(BlueprintPure)
	void GetAllHeadItemId(TArray<FPrimaryAssetId>& AssetIds);

	UFUNCTION(BlueprintPure)
	void GetAllUpperItemId(TArray<FPrimaryAssetId>& AssetIds);

	UFUNCTION(BlueprintPure)
	void GetAllLowerItemId(TArray<FPrimaryAssetId>& AssetIds);
	
	UFUNCTION(BlueprintPure)
	void GetAllTrinketItemId(TArray<FPrimaryAssetId>& AssetIds);

	UFUNCTION(BlueprintPure)
	void GetAllItemId(TArray<FPrimaryAssetId>& AssetIds);

	UFUNCTION(BlueprintPure)
	void GetAllEquipmentId(TArray<FPrimaryAssetId>& AssetIds);
	
	UFUNCTION(BlueprintCallable)
	void GetAssetID(UEquipmentItemData* ItemData);

	UFUNCTION(BlueprintCallable)
	TArray<UBaseItem*> GetAllItemsOfSlotType(EEquipmentSlot SlotType);
	
	void GetAllIdOfType(FName InAssetType, TArray<FPrimaryAssetId>& OutAssetId);
	TArray<FPrimaryAssetType> GetAssetTypes();
	TArray<UBaseItemData> GetItemData(TArray<FPrimaryAssetId> AssetIds);

	UPxiiInventorySubsystem* GetSubsystem();

private:

	UPROPERTY()
	UPxiiInventorySubsystem* subsystem;
};
