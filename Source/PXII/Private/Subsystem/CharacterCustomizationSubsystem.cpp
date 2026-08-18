#include "Subsystem/CharacterCustomizationSubsystem.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Subsystem/PxiiSaveSubsystem.h"

FString UCharacterCustomizationSubsystem::GetHeadSlotKey()
{
	FString key = "";
	if(!CustomizationMap)
	{
		return key;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->HeadSlot;
	return slot->SlotKey;
}

FString UCharacterCustomizationSubsystem::GetUpperSlotKey()
{
	FString key = "";
	if(!CustomizationMap)
	{
		return key;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->UpperBodySlot;
	return slot->SlotKey;
}

FString UCharacterCustomizationSubsystem::GetLowerSlotKey()
{
	FString key = "";
	if(!CustomizationMap)
	{
		return key;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->LowerBodySlot;
	return slot->SlotKey;
}

TArray<UPxiiCustomizationBaseData*> UCharacterCustomizationSubsystem::GetHeadData()
{
	TArray<UPxiiCustomizationBaseData*> resultMap;
	if(!CustomizationMap)
	{
		return resultMap;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->HeadSlot;
	return slot->ItemData;
}

TArray<UPxiiCustomizationBaseData*> UCharacterCustomizationSubsystem::GetUpperData()
{
	TArray<UPxiiCustomizationBaseData*> resultMap;
	if(!CustomizationMap)
	{
		return resultMap;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->UpperBodySlot;
	return slot->ItemData;
}

TArray<UPxiiCustomizationBaseData*> UCharacterCustomizationSubsystem::GetLowerData()
{
	TArray<UPxiiCustomizationBaseData*> resultMap;
	if(!CustomizationMap)
	{
		return resultMap;
	}
	
	UPxiiCustomizationSlot* slot = CustomizationMap->HeadSlot;
	return slot->ItemData;
}

void UCharacterCustomizationSubsystem::ProcessCustomizationMap_Implementation()
{
}

bool UCharacterCustomizationSubsystem::FindHeadData_Implementation(UPxiiCustomizationBaseData*& headData,
                                                                   const FString& itemKey)
{
	if(!CustomizationMap)
	{
		return false;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->HeadSlot;
	const TArray<UPxiiCustomizationBaseData*>& data = slot->ItemData;

	int32 index = data.IndexOfByPredicate([&itemKey](const UPxiiCustomizationBaseData* item)
	{
		return item->ItemKey == itemKey;
	});

	if(index == INDEX_NONE)
	{
		return false;
	}

	headData = data[index];
	return true;
}

bool UCharacterCustomizationSubsystem::FindUpperData_Implementation(UPxiiCustomizationBaseData*& UpperData,
	const FString& itemKey)
{
	if(!CustomizationMap)
	{
		return false;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->UpperBodySlot;
	const TArray<UPxiiCustomizationBaseData*>& data = slot->ItemData;

	int32 index = data.IndexOfByPredicate([&itemKey](const UPxiiCustomizationBaseData* item)
	{
		return item->ItemKey == itemKey;
	});

	if(index == INDEX_NONE)
	{
		return false;
	}

	UpperData = data[index];

	return false;
}

bool UCharacterCustomizationSubsystem::FindLowerData_Implementation(UPxiiCustomizationBaseData*& LowerData,
	const FString& itemKey)
{
	if(!CustomizationMap)
	{
		return false;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->LowerBodySlot;
	const TArray<UPxiiCustomizationBaseData*>& data = slot->ItemData;

	int32 index = data.IndexOfByPredicate([&itemKey](const UPxiiCustomizationBaseData* item)
	{
		return item->ItemKey == itemKey;
	});

	if(index == INDEX_NONE)
	{
		return false;
	}

	LowerData = data[index];

	return false;
}
