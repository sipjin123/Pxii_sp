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

	UPROPERTY()
	TArray<TObjectPtr<UEquipmentSlot>> Slots;
};

UCLASS()
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
	void UnequipItem(EEquipmentSlot slotType);
	void UnequipItem_Implementation(EEquipmentSlot slotType);

	UFUNCTION(BlueprintPure)
	UEquipmentSlot* GetAvailableSlot(EEquipmentSlot slotType, bool ignoreAvailability = false);

	UFUNCTION(BlueprintPure)
	bool IsSlotOccupied(EEquipmentSlot slotType);

protected:
	UEquipmentSlot* CreateEquipmentSlot(EEquipmentSlot slot, int32 slotINdex);

private:

	UPROPERTY()
	TMap<EEquipmentSlot, FEquipmentSlotContainer> EquipmentSlots;
};
