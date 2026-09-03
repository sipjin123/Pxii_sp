#include "Item/EquipmentItem.h"

#include "Utility/PXIILogUtility.h"

void UEquipmentItem::Initialize(FItemSaveData saveData)
{
	Super::Initialize(saveData);
	EquipmentItemData = Cast<UEquipmentItemData>(BaseItemData);
	if(EquipmentItemData)
	{
		PXII_LOG(ELogCategory::Equipment, Log, TEXT("Equip item Initialize ID: %s"), *EquipmentItemData->GetPrimaryAssetId().ToString());
	}
}

void UEquipmentItem::Initialize(FPrimaryAssetId assetId)
{
	Super::Initialize(assetId);
}

UBaseItemData* UEquipmentItem::GetData()
{
	return EquipmentItemData;
}

FItemSaveData UEquipmentItem::GetSaveData()
{
	FItemSaveData data = FItemSaveData();
	data.AssetId = EquipmentItemData->GetPrimaryAssetId();
	data.InstanceId = GetInstanceId();
	data.Quantity = GetItemQuantity();

	return data;
}

UEquipmentItemData* UEquipmentItem::GetEquipmentData()
{
	return EquipmentItemData;
}
