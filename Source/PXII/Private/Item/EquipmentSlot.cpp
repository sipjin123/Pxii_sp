#include "Item/EquipmentSlot.h"
#include "Item/EquipmentItem.h"
#include "Utility/PXIILogUtility.h"

void UEquipmentSlot::Initialize(EEquipmentSlot InSlotType, int32 InSlotIndex)
{
	SlotIndex = InSlotIndex;
	SlotType = InSlotType;
}

void UEquipmentSlot::LoadSaveData(FEquipmentSlotSaveData saveData)
{
	if(saveData.isEquipped)
	{
		UEquipmentItem* item = NewObject<UEquipmentItem>(this);
		item->Initialize(saveData);
		EquipItem(item);
	}
}

bool UEquipmentSlot::EquipItem(UEquipmentItem* InEquipmentItem)
{
	//TODO(DZ) : Handle Equip of GE/GA if any

	EquippedItem = InEquipmentItem;
	IsOccupied = true;

	PXII_LOG(ELogCategory::Equipment, Log, TEXT("Item Name: %s"), *EquippedItem->GetData()->GetPrimaryAssetId().ToString());

	OnSlotUpdated.Broadcast();
	
	return true;
}

void UEquipmentSlot::UnequipItem()
{
	//TODO(DZ) : Handle Unequip of GE/GA if any
	IsOccupied = false;
	EquippedItem = nullptr;

	if(!EquippedItem)
	{
		PXII_LOG(ELogCategory::Equipment, Log, TEXT("Item unequipped"));
	}

	OnSlotUpdated.Broadcast();
}

bool UEquipmentSlot::IsSlotOccupied() const
{
	return IsOccupied;
}

UEquipmentItemData* UEquipmentSlot::GetEquippedItem() const
{
	if(!IsSlotOccupied())
	{
		return nullptr;
	}
	
	return EquippedItem->GetEquipmentData();
}

EEquipmentSlot UEquipmentSlot::GetSlotType() const
{
	return SlotType;
}

int32 UEquipmentSlot::GetSlotIndex() const
{
	return SlotIndex;
}

FEquipmentSlotSaveData UEquipmentSlot::GetSaveData()
{
	FEquipmentSlotSaveData data = FEquipmentSlotSaveData();
	data.isEquipped = IsSlotOccupied();
	data.SlotIndex = GetSlotIndex();
	data.Quantity = 0;
	if(EquippedItem)
	{
		data.Quantity = 1;
		data.AssetId = EquippedItem->GetData()->GetPrimaryAssetId();
		data.InstanceId = EquippedItem->GetInstanceId();		
	}

	return data;
}
