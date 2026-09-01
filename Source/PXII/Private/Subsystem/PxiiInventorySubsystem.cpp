#include "Subsystem/PxiiInventorySubsystem.h"
#include "Engine/AssetManager.h"

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

void UPxiiInventorySubsystem::AddItemToInventory_Implementation(UBaseItemData* itemData)
{
	UBaseItem* item =  NewObject<UBaseItem>(this);
	item->Initialize(itemData->AssetId);
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


