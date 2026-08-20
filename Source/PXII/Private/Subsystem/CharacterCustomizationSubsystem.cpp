#include "Subsystem/CharacterCustomizationSubsystem.h"
#include "Engine/World.h"
#include "Subsystem/PxiiSaveSubsystem.h"
#include "Utility/PXIILogUtility.h"

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

FString UCharacterCustomizationSubsystem::GetTattooSlotKey()
{
	FString key = "";
	if(!CustomizationMap)
	{
		return key;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->TattooSlot;
	return slot->SlotKey;
}

FString UCharacterCustomizationSubsystem::GetBlasterSlotKey()
{
	FString key = "";
	if(!CustomizationMap)
	{
		return key;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->BlasterSlot;
	return slot->SlotKey;
}

FString UCharacterCustomizationSubsystem::GetHeadSecondarySlotKey()
{
	FString key = "";
	if(!CustomizationMap)
	{
		return key;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->HeadSlot;
	return slot->SecondarySlotKey;
}

FString UCharacterCustomizationSubsystem::GetUpperSecondarySlotKey()
{
	FString key = "";
	if(!CustomizationMap)
	{
		return key;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->UpperBodySlot;
	return slot->SecondarySlotKey;
}

FString UCharacterCustomizationSubsystem::GetLowerSecondarySlotKey()
{
	FString key = "";
	if(!CustomizationMap)
	{
		return key;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->LowerBodySlot;
	return slot->SecondarySlotKey;
}

FString UCharacterCustomizationSubsystem::GetTattooSecondarySlotKey()
{
	FString key = "";
	if(!CustomizationMap)
	{
		return key;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->TattooSlot;
	return slot->SecondarySlotKey;
}

FString UCharacterCustomizationSubsystem::GetBlasterSecondarySlotKey()
{
	FString key = "";
	if(!CustomizationMap)
	{
		return key;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->BlasterSlot;
	return slot->SecondarySlotKey;
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
	
	UPxiiCustomizationSlot* slot = CustomizationMap->LowerBodySlot;
	return slot->ItemData;
}

TArray<UPxiiCustomizationBaseData*> UCharacterCustomizationSubsystem::GetTattooData()
{
	TArray<UPxiiCustomizationBaseData*> resultMap;
	if(!CustomizationMap)
	{
		return resultMap;
	}
	
	UPxiiCustomizationSlot* slot = CustomizationMap->TattooSlot;
	return slot->ItemData;
}

TArray<UPxiiCustomizationBaseData*> UCharacterCustomizationSubsystem::GetBlasterData()
{
	TArray<UPxiiCustomizationBaseData*> resultMap;
	if(!CustomizationMap)
	{
		return resultMap;
	}
	
	UPxiiCustomizationSlot* slot = CustomizationMap->BlasterSlot;
	return slot->ItemData;
}

void UCharacterCustomizationSubsystem::Initialize(UPxiiCustomizationMap* InCustomizationMap)
{
	CustomizationMap = InCustomizationMap;
	
	PXII_LOG(ELogCategory::General, Log, TEXT("Customization Initialized"));
}

void UCharacterCustomizationSubsystem::SetCustomizationHandler(APxiiCustomizationHandler* InCustomizationHandler)
{
	CustomizationHandler = InCustomizationHandler;
	
	PXII_LOG(ELogCategory::General, Log, TEXT("Customization Handler Initialized"));
}

void UCharacterCustomizationSubsystem::ProcessCustomizationMap_Implementation()
{
}

void UCharacterCustomizationSubsystem::OpenCustomizationLevel(bool closeIfActive)
{
	if(!CustomizationHandler)
	{
		return;
	}

	if(CustomizationHandler->GetCustomizationActive())
	{
		if(closeIfActive)
		{
			CloseCustomizationLevel();
		}
		return;
	}

	CustomizationHandler->OpenCustomization();
}

void UCharacterCustomizationSubsystem::CloseCustomizationLevel()
{
	if(!CustomizationHandler)
	{
		return;
	}

	CustomizationHandler->CloseCustomization();
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

bool UCharacterCustomizationSubsystem::FindTattooData_Implementation(UPxiiCustomizationBaseData*& TattooData,
	const FString& itemKey)
{
	if(!CustomizationMap)
	{
		return false;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->TattooSlot;
	const TArray<UPxiiCustomizationBaseData*>& data = slot->ItemData;

	int32 index = data.IndexOfByPredicate([&itemKey](const UPxiiCustomizationBaseData* item)
	{
		return item->ItemKey == itemKey;
	});

	if(index == INDEX_NONE)
	{
		return false;
	}

	TattooData = data[index];

	return false;
}

bool UCharacterCustomizationSubsystem::FindBlasterData_Implementation(UPxiiCustomizationBaseData*& BlasterData,
                                                                      const FString& itemKey)
{
	if(!CustomizationMap)
	{
		return false;
	}

	UPxiiCustomizationSlot* slot = CustomizationMap->BlasterSlot;
	const TArray<UPxiiCustomizationBaseData*>& data = slot->ItemData;

	int32 index = data.IndexOfByPredicate([&itemKey](const UPxiiCustomizationBaseData* item)
	{
		return item->ItemKey == itemKey;
	});

	if(index == INDEX_NONE)
	{
		return false;
	}

	BlasterData = data[index];

	return false;
}

APxiiCustomizationHandler* UCharacterCustomizationSubsystem::GetCustomizationHandler()
{
	return CustomizationHandler;
}
