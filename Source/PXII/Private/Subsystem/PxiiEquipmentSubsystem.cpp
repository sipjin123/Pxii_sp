#include "Subsystem/PxiiEquipmentSubsystem.h"
#include "Subsystem/PxiiSaveSubsystem.h"

void UPxiiEquipmentSubsystem::Initialize()
{
	FEquipmentSlotContainer Blaster = FEquipmentSlotContainer();
	Blaster.Slots.Add(CreateEquipmentSlot(EEquipmentSlot::Blaster, 0));

	FEquipmentSlotContainer ArmBlade = FEquipmentSlotContainer();
	ArmBlade.Slots.Add(CreateEquipmentSlot(EEquipmentSlot::ArmBlade, 0));

	FEquipmentSlotContainer Head = FEquipmentSlotContainer();
	Head.Slots.Add(CreateEquipmentSlot(EEquipmentSlot::Head, 0));

	FEquipmentSlotContainer Upper = FEquipmentSlotContainer();
	Upper.Slots.Add(CreateEquipmentSlot(EEquipmentSlot::Upper, 0));

	FEquipmentSlotContainer Lower = FEquipmentSlotContainer();
	Lower.Slots.Add(CreateEquipmentSlot(EEquipmentSlot::Lower, 0));

	FEquipmentSlotContainer Trinket = FEquipmentSlotContainer();
	Trinket.Slots.Add(CreateEquipmentSlot(EEquipmentSlot::Trinket, 0));
	Trinket.Slots.Add(CreateEquipmentSlot(EEquipmentSlot::Trinket, 1));
	Trinket.Slots.Add(CreateEquipmentSlot(EEquipmentSlot::Trinket, 1));


	EquipmentSlots.Add(EEquipmentSlot::Blaster, Blaster);
	EquipmentSlots.Add(EEquipmentSlot::ArmBlade, ArmBlade);
	EquipmentSlots.Add(EEquipmentSlot::Upper, Upper);
	EquipmentSlots.Add(EEquipmentSlot::Lower, Lower);
	EquipmentSlots.Add(EEquipmentSlot::Head, Head);
	EquipmentSlots.Add(EEquipmentSlot::Trinket, Trinket);

}

void UPxiiEquipmentSubsystem::FillEquipmentSaveData(FEquipmentSaveData& saveData)
{
	saveData.EquipmentSaveData.Empty();
	for(const auto& itemData : EquipmentSlots)
	{
		FEquipmentSlotSaveContainer slotContainer;
		for(const auto& slotData : itemData.Value.Slots)
		{
			slotContainer.Slots.Add(slotData->GetSaveData());
		}
		saveData.EquipmentSaveData.Add(itemData.Key, slotContainer);
	}
}

void UPxiiEquipmentSubsystem::LoadEquipmentSaveData_Implementation(const FEquipmentSaveData& saveData)
{
	for(const auto& itemData : saveData.EquipmentSaveData)
	{
		if(!EquipmentSlots.Contains(itemData.Key))
		{
			continue;
		}

		const auto& slotSaveData = itemData.Value;
		FEquipmentSlotContainer slots = EquipmentSlots[itemData.Key];
		for(const auto& slot : slots.Slots)
		{
			if(!slotSaveData.HasSlot(slot->GetSlotIndex()))
			{
				continue;
			}

			FEquipmentSlotSaveData data = slotSaveData.GetSlot(slot->GetSlotIndex());
			slot->LoadSaveData(data);
		}
	}
}

bool UPxiiEquipmentSubsystem::EquipItem_Implementation(EEquipmentSlot slotType, UBaseItem* item)
{
	UEquipmentSlot* slot = GetAvailableSlot(slotType);
	if(!slot)
	{
		return false;
	}

	UEquipmentItem* equipmentItem  = NewObject<UEquipmentItem>(this);
	equipmentItem->Initialize(item->GetSaveData());
	return slot->EquipItem(equipmentItem);
}

bool UPxiiEquipmentSubsystem::SwapItem_Implementation(EEquipmentSlot slotType, UBaseItem* item)
{
	UEquipmentSlot* slot = GetAvailableSlot(slotType, true);
	if(!slot)
	{
		return false;
	}

	if(slot->IsSlotOccupied())
	{
		slot->UnequipItem();
	}

	UEquipmentItem* equipmentItem  = NewObject<UEquipmentItem>(this);
	equipmentItem->Initialize(item->GetSaveData());
	return slot->EquipItem(equipmentItem);
}

void UPxiiEquipmentSubsystem::UnequipItem_Implementation(EEquipmentSlot slotType)
{
	
}

UEquipmentSlot* UPxiiEquipmentSubsystem::GetAvailableSlot(EEquipmentSlot slotType, bool ignoreAvailability)
{
	if(!EquipmentSlots.Contains(slotType))
	{
		return nullptr;
	}

	FEquipmentSlotContainer* itemSlots = EquipmentSlots.Find(slotType);
	if(itemSlots->Slots.IsEmpty())
	{
		return nullptr;
	}
	
	TObjectPtr<UEquipmentSlot>* FoundSlot = itemSlots->Slots.FindByPredicate(
	[](const TObjectPtr<UEquipmentSlot>& Slot)
		{
			return !Slot->IsSlotOccupied();
		}
	);

	if(FoundSlot)
	{
		return FoundSlot->Get();
	}

	if(ignoreAvailability)
	{
		return itemSlots->Slots[0].Get();
	}

	return nullptr;
}

bool UPxiiEquipmentSubsystem::IsSlotOccupied(EEquipmentSlot slotType)
{
	if(!EquipmentSlots.Contains(slotType))
	{
		return false;	
	}

	FEquipmentSlotContainer* itemSlots = EquipmentSlots.Find(slotType);
	if(itemSlots->Slots.IsEmpty())
	{
		return false;
	}
	
	bool bHasEquipped = itemSlots->Slots.ContainsByPredicate(
		[](const TObjectPtr<UEquipmentSlot>& Slot)
		{
			return Slot && Slot->IsSlotOccupied();
		}
	);

	return bHasEquipped;
}

UEquipmentSlot* UPxiiEquipmentSubsystem::CreateEquipmentSlot(EEquipmentSlot slot, int32 slotIndex)
{
	UEquipmentSlot* equipmentSlot = NewObject<UEquipmentSlot>(this);
	equipmentSlot->Initialize(slot, slotIndex);
	return equipmentSlot;
}
