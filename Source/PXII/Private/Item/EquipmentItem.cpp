#include "Item/EquipmentItem.h"

void UEquipmentItem::Initialize(FItemSaveData saveData)
{
	Super::Initialize(saveData);
	EquipmentItemData = Cast<UEquipmentItemData>(GetData());
}

void UEquipmentItem::Initialize(FPrimaryAssetId assetId)
{
	Super::Initialize(assetId);
}

FItemSaveData UEquipmentItem::GetSaveData()
{
	FItemSaveData data = FItemSaveData();
	data.AssetId = EquipmentItemData->AssetId;
	data.InstanceId = GetInstanceId();
	data.Quantity = GetItemQuantity();

	return data;
}

UBaseItemData* UEquipmentItem::GetData()
{
	return EquipmentItemData;
}
