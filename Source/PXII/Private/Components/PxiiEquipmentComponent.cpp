#include "Components/PxiiEquipmentComponent.h"
#include "Subsystem/PxiiEquipmentSubsystem.h"

void UPxiiEquipmentComponent::FillSaveData(FEquipmentSaveData& saveData)
{
	
	GetSubsystem()->FillEquipmentSaveData(saveData);
}

UPxiiEquipmentSubsystem* UPxiiEquipmentComponent::GetSubsystem()
{
	if(subsystem)
	{
		return subsystem;
	}
	
	UWorld* world = GetWorld();
	if(!world)
	{
		return nullptr;
	}
	
	subsystem = world->GetGameInstance()->GetSubsystem<UPxiiEquipmentSubsystem>();
	return subsystem;
}
