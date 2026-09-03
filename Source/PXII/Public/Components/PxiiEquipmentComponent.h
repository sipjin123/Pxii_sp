#pragma once
#include "Components/ActorComponent.h"
#include "Item/EquipmentSlot.h"
#include "SaveData/PxiiSaveData.h"
#include "Subsystem/PxiiEquipmentSubsystem.h"
#include "PxiiEquipmentComponent.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	void FillSaveData(FEquipmentSaveData& saveData);

	UFUNCTION(BlueprintPure)
	UPxiiEquipmentSubsystem* GetSubsystem();

private:

	UPROPERTY()
	TObjectPtr<UPxiiEquipmentSubsystem> subsystem;
};
