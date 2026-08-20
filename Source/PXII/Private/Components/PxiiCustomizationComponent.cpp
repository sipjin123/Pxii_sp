#include "Components/PxiiCustomizationComponent.h"

#include "SaveData/PxiiSaveData.h"
#include "Subsystem/CharacterCustomizationSubsystem.h"
#include "Subsystem/PxiiSaveSubsystem.h"

void UPxiiCustomizationComponent::SetHeadSlot_Implementation(UPxiiCustomizationBaseData* headData, bool saveChange)
{
	CurrentHead = headData;
	OnSlotUpdated(saveChange);
}

void UPxiiCustomizationComponent::SetLowerSlot_Implementation(UPxiiCustomizationBaseData* lowerData, bool saveChange)
{
	CurrentLower = lowerData;
	OnSlotUpdated(saveChange);
}

void UPxiiCustomizationComponent::SetUpperSlot_Implementation(UPxiiCustomizationBaseData* upperData, bool saveChange)
{
	CurrentUpper = upperData;
	OnSlotUpdated(saveChange);
}

void UPxiiCustomizationComponent::SetTattooSlot_Implementation(UPxiiCustomizationBaseData* tattooData, bool saveChange)
{
	CurrentTattoo = tattooData;
	OnSlotUpdated(saveChange);
}

void UPxiiCustomizationComponent::SetBlasterSlot_Implementation(UPxiiCustomizationBaseData* blasterData, bool saveChange)
{
	CurrentBlaster = blasterData;
	OnSlotUpdated(saveChange);
}

void UPxiiCustomizationComponent::OnSlotUpdated_Implementation(bool saveChange)
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

UPxiiCustomizationBaseData* UPxiiCustomizationComponent::GetCurrentBlaster()
{
	return CurrentBlaster;
}

UPxiiCustomizationBaseData* UPxiiCustomizationComponent::GetCurrentTattoo()
{
	return CurrentTattoo;
}

void UPxiiCustomizationComponent::FillCustomizationSaveData(FCustomizationSaveData& data)
{
	FCustomizationSaveData save;
	save.HeadSlotItemKey = GetCurrentHead()->ItemKey;
	save.UpperSlotItemKey = GetCurrentUpper()->ItemKey;
	save.LowerSlotItemKey = GetCurrentLower()->ItemKey;
	save.TattooSlotKey = GetCurrentTattoo()->ItemKey;
	save.BlasterSlotKey = GetCurrentBlaster()->ItemKey;

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
	UPxiiCustomizationBaseData* blasterData;
	UPxiiCustomizationBaseData* tattooData;


	if(CustomizationSubsystem->FindHeadData(headData, saveData.HeadSlotItemKey))
	{
		SetHeadSlot(headData, false);
	}
	
	if(CustomizationSubsystem->FindUpperData(upperData, saveData.UpperSlotItemKey))
	{
		SetUpperSlot(upperData, false);
	}

	if(CustomizationSubsystem->FindLowerData(lowerData, saveData.LowerSlotItemKey))
	{
		SetLowerSlot(lowerData, false);
	}

	if(CustomizationSubsystem->FindBlasterData(blasterData, saveData.LowerSlotItemKey))
	{
		SetBlasterSlot(blasterData, false);
	}

	if(CustomizationSubsystem->FindTattooData(tattooData, saveData.LowerSlotItemKey))
	{
		SetTattooSlot(tattooData, false);
	}
}
