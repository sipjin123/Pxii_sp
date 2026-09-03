#include "Subsystem/PxiiInventorySubsystem.h"
#include "Engine/AssetManager.h"
#include "Utility/PXIILogUtility.h"

void UPxiiInventorySubsystem::Initialize()
{
	
}

void UPxiiInventorySubsystem::LoadItemInventory_Implementation(const FInventorySaveData& inventoryData)
{
	ItemData.Empty();
	for (const auto& item : inventoryData.Items)
	{
		UBaseItem* baseItem = NewObject<UBaseItem>(this);
		baseItem->Initialize(item);
		ItemData.Add(baseItem->GetInstanceId(), baseItem);
	}
}

void UPxiiInventorySubsystem::FillSaveData_Implementation(FInventorySaveData& inventoryData)
{
	inventoryData.Items.Empty();
	for (auto item : ItemData)
	{
		inventoryData.Items.Add(item.Value->GetSaveData());
	}
}

void UPxiiInventorySubsystem::AddItemToInventory_Implementation(UBaseItemData* inItemData)
{
	if(TryAddItemStack(inItemData))
	{
		PXII_LOG(ELogCategory::Inventory, Log, TEXT("Item is added as stack"));
		return;
	}
	
	UBaseItem* item =  NewObject<UBaseItem>(this);
	item->Initialize(inItemData->GetPrimaryAssetId());
	ItemData.Add(item->GetInstanceId(), item);
}

void UPxiiInventorySubsystem::RemoveItemToInventory_Implementation(FGuid instanceId)
{
	if(!ItemData.Contains(instanceId))
	{
		return;
	}

	ItemData.Remove(instanceId);
}

bool UPxiiInventorySubsystem::TryAddItemStack(UBaseItemData* InItemData)
{
	bool isSuccess = false;
	TArray<UBaseItem*> potentialItem = GetAllItemWithAssetId(InItemData->GetPrimaryAssetId());
	for (UBaseItem* item : potentialItem)
	{
		if(!item->CanStack())
		{
			continue;
		}

		item->AddStack();
		isSuccess = true;
	}

	return isSuccess;
}

TArray<UBaseItem*> UPxiiInventorySubsystem::GetAllItemWithAssetId(FPrimaryAssetId assetId)
{
	TArray<UBaseItem*> filteredItem;
	for(const auto& item : ItemData)
	{
		if(item.Value->GetData()->GetPrimaryAssetId() == assetId)
		{
			filteredItem.Add(item.Value);
		}
	}
	
	return filteredItem;
}

bool UPxiiInventorySubsystem::GetAllItems(TArray<UBaseItem*>& OutResult)
{
	ItemData.GenerateValueArray(OutResult);
	return !ItemData.IsEmpty();
}


