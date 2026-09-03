#pragma once
#include "EquipmentItem.h"
#include "EquipmentItemData.h"
#include "SaveData/PxiiSaveData.h"
#include "EquipmentSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlotUpdated);

UCLASS(BlueprintType)
class PXII_API UEquipmentSlot : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnSlotUpdated OnSlotUpdated;
	
	void Initialize(EEquipmentSlot InSlotType, int32 InSlotIndex);

	void LoadSaveData(FEquipmentSlotSaveData saveData);

	bool EquipItem(UEquipmentItem* InEquipmentItem);;

	void UnequipItem();

	UFUNCTION(BlueprintPure)
	bool IsSlotOccupied() const;

	UFUNCTION(BlueprintPure)
	UEquipmentItemData* GetEquippedItem() const;

	EEquipmentSlot GetSlotType() const;

	UFUNCTION(BlueprintPure)
	int32 GetSlotIndex() const;

	FEquipmentSlotSaveData GetSaveData();

private:
	
	UPROPERTY()
	EEquipmentSlot SlotType;

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	bool IsOccupied = false;

	UPROPERTY()
	TObjectPtr<UEquipmentItem> EquippedItem;
};
