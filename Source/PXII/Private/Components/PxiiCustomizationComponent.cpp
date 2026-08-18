#include "Components/PxiiCustomizationComponent.h"

#include "SaveData/PxiiSaveData.h"
#include "Subsystem/CharacterCustomizationSubsystem.h"
#include "Subsystem/PxiiSaveSubsystem.h"

void UPxiiCustomizationComponent::SetHeadSlot_Implementation(UPxiiCustomizationBaseData* headData)
{
	CurrentHead = headData;
	OnSlotUpdated();
}

void UPxiiCustomizationComponent::SetLowerSlot_Implementation(UPxiiCustomizationBaseData* lowerData)
{
	CurrentLower = lowerData;
	OnSlotUpdated();
}

void UPxiiCustomizationComponent::SetUpperSlot_Implementation(UPxiiCustomizationBaseData* upperData)
{
	CurrentUpper = upperData;
	OnSlotUpdated();
}

void UPxiiCustomizationComponent::OnSlotUpdated_Implementation()
{
}

UPxiiCustomizationBaseData* UPxiiCustomizationComponent::GetCurrentHead()
{
	return CurrentHead;
}

UPxiiCustomizationBaseData* UPxiiCustomizationComponent::GetCurrentUpper()
{
	return CurrentUpper;
}

UPxiiCustomizationBaseData* UPxiiCustomizationComponent::GetCurrentLower()
{
	return CurrentLower;
}

void UPxiiCustomizationComponent::FillCustomizationSaveData(FCustomizationSaveData& data)
{
	FCustomizationSaveData save;
	save.HeadSlotItemKey = GetCurrentHead()->ItemKey;
	save.UpperSlotItemKey = GetCurrentUpper()->ItemKey;
	save.LowerSlotItemKey = GetCurrentLower()->ItemKey;

	data = save;
}

void UPxiiCustomizationComponent::LoadCustomization()
{
	UWorld* world = GetWorld();
	if(!world)
	{
		return;
	}
	
	UPxiiSaveSubsystem* saveSubsystem = world->GetGameInstance()->GetSubsystem<UPxiiSaveSubsystem>();

	if(!saveSubsystem)
	{
		return;
	}

	saveSubsystem->LoadCustomization(this);
}

void UPxiiCustomizationComponent::LoadData(const FCustomizationSaveData& saveData)
{
	UWorld* world = GetWorld();
	if(!world)
	{
		return;
	}
	
	UCharacterCustomizationSubsystem* CustomizationSubsystem = world->GetGameInstance()->GetSubsystem<UCharacterCustomizationSubsystem>();

	if(!CustomizationSubsystem)
	{
		return;
	}

	UPxiiCustomizationBaseData* headData;
	UPxiiCustomizationBaseData* upperData;
	UPxiiCustomizationBaseData* lowerData;

	if(CustomizationSubsystem->FindHeadData(headData, saveData.HeadSlotItemKey))
	{
		CurrentHead = headData;
	}
	
	if(CustomizationSubsystem->FindUpperData(upperData, saveData.UpperSlotItemKey))
	{
		CurrentUpper = upperData;
	}

	if(CustomizationSubsystem->FindLowerData(lowerData, saveData.LowerSlotItemKey))
	{
		CurrentLower = lowerData;
	}
}
