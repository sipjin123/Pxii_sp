#pragma once
#include "Item/EquipmentItemData.h"
#include "Item/EquipmentSlot.h"
#include "SaveData/PxiiSaveData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PxiiEquipmentSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentEquipped, EEquipmentSlot, EquipmentSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipmentUnequipped, EEquipmentSlot, EquipmentSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, EEquipmentSlot, EquipmentSlot, bool, isOccupied);

USTRUCT(BlueprintType)
struct FEquipmentSlotContainer
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UEquipmentSlot>> Slots;
};

UCLASS(BlueprintType)
class PXII_API UPxiiEquipmentSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentEquipped OnEquipmentEquipped;
	
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentUnequipped OnEquipmentUnequipped;

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentChanged OnEquipmentChanged;

	UFUNCTION(BlueprintCallable)
	void Initialize();
	void FillEquipmentSaveData(FEquipmentSaveData& saveData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LoadEquipmentSaveData(const FEquipmentSaveData& saveData);
	void LoadEquipmentSaveData_Implementation(const FEquipmentSaveData& saveData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool EquipItem(EEquipmentSlot slotType, UBaseItem* item); 
	bool EquipItem_Implementation(EEquipmentSlot slotType, UBaseItem* item);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool SwapItem(EEquipmentSlot slotType, UBaseItem* item); 
	bool SwapItem_Implementation(EEquipmentSlot slotType, UBaseItem* item);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool UnequipItem(EEquipmentSlot slotType, int32 slotIndex);
	bool UnequipItem_Implementation(EEquipmentSlot slotType, int32 slotIndex);

	UFUNCTION(BlueprintPure)
	UEquipmentSlot* GetAvailableSlot(EEquipmentSlot slotType, bool ignoreAvailability = false);

	UFUNCTION(BlueprintPure)
	bool IsSlotOccupied(EEquipmentSlot slotType);

	UFUNCTION(BlueprintPure)
	FEquipmentSlotContainer GetSlotContainerOfType(EEquipmentSlot slotType);

	UFUNCTION(BlueprintPure)
	UEquipmentSlot* GetSlotIndexOfType(EEquipmentSlot SlotType, int32 slotIndex);
	
protected:
	UEquipmentSlot* CreateEquipmentSlot(EEquipmentSlot slot, int32 slotINdex);

private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<EEquipmentSlot, FEquipmentSlotContainer> EquipmentSlots;
};
