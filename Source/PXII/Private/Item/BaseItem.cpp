#include "Item/BaseItem.h"
#include "Engine/AssetManager.h"

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

int32 UBaseItem::GetItemQuantity()
{
	return Quantity;
}

FItemSaveData UBaseItem::GetSaveData()
{
	FItemSaveData data = FItemSaveData();
	data.AssetId = BaseItemData->AssetId;
	data.InstanceId = GetInstanceId();
	data.Quantity = GetItemQuantity();

	return data;
}

UBaseItemData* UBaseItem::GetItemData(FPrimaryAssetId assetId)
{
	UAssetManager& manager = UAssetManager::Get();
	FSoftObjectPath assetPath = manager.GetPrimaryAssetPath(assetId);

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


