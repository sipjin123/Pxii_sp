#include "Item/BaseItem.h"
#include "Engine/AssetManager.h"
#include "Utility/PXIILogUtility.h"

void UBaseItem::Initialize(FPrimaryAssetId assetId)
{
	BaseItemData = GetItemData(assetId);
	InstanceId = FGuid::NewGuid();
	Quantity = 1;
}

void UBaseItem::Initialize(FItemSaveData saveData)
{
	InstanceId = saveData.InstanceId;
	BaseItemData = GetItemData(saveData.AssetId);
	Quantity = saveData.Quantity;
}

FGuid UBaseItem::GetInstanceId()
{
	return InstanceId;
}

UBaseItemData* UBaseItem::GetData()
{
	return BaseItemData;
}

int32 UBaseItem::GetItemQuantity() const
{
	return Quantity;
}

void UBaseItem::AddStack()
{
	Quantity++;
}

FItemSaveData UBaseItem::GetSaveData()
{
	FItemSaveData data = FItemSaveData();
	data.AssetId = BaseItemData->GetPrimaryAssetId();
	data.InstanceId = GetInstanceId();
	data.Quantity = GetItemQuantity();

	PXII_LOG(ELogCategory::Inventory, Log, TEXT("SAVED DATA: ID: %s | InstanceID: %s | Quantity: %d"),
		*data.AssetId.ToString(), *data.InstanceId.ToString(), data.Quantity);

	return data;
}

bool UBaseItem::CanStack() const
{
	return GetItemQuantity() < BaseItemData->MaxStackSize;
}

UBaseItemData* UBaseItem::GetItemData(FPrimaryAssetId assetId)
{
	UAssetManager& manager = UAssetManager::Get();
	FSoftObjectPath assetPath = manager.GetPrimaryAssetPath(assetId);
	PXII_LOG(ELogCategory::Inventory, Log, TEXT("ASSET ID: %s"), *assetId.ToString());
	if (assetPath.IsValid())
	{
		UBaseItemData* ItemData = Cast<UBaseItemData>(assetPath.TryLoad());
		if (ItemData)
		{
			UE_LOG(LogTemp, Log, TEXT("Item: %s"), *ItemData->ItemDisplayName.ToString());
			return ItemData;
		}
	}

	return nullptr;
}


