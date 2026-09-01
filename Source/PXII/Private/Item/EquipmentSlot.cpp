#include "Item/EquipmentSlot.h"
#include "Item/EquipmentItem.h"

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

	return true;
}

void UEquipmentSlot::UnequipItem()
{
	//TODO(DZ) : Handle Unequip of GE/GA if any
	IsOccupied = false;
	EquippedItem = nullptr;
}

bool UEquipmentSlot::IsSlotOccupied() const
{
	return IsOccupied;
}

UEquipmentItemData* UEquipmentSlot::GetEquippedItem() const
{
	if(IsSlotOccupied())
	{
		return nullptr;
	}

	UEquipmentItemData* data = Cast<UEquipmentItemData>(EquippedItem->GetData());
	return data;
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
	data.Quantity = 1;
	data.AssetId = EquippedItem->GetData()->AssetId;
	data.InstanceId = EquippedItem->GetInstanceId();

	return data;
}
