#include "Components/PxiiInventoryComponent.h"
#include "Engine/AssetManager.h"
#include "Subsystem/PxiiInventorySubsystem.h"
#include "Utility/PXIILogUtility.h"

void UPxiiInventoryComponent::FillSaveData(FInventorySaveData& saveData)
{
	GetSubsystem()->FillSaveData(saveData);
}

void UPxiiInventoryComponent::GetAllBlasterItemId(TArray<FPrimaryAssetId>& AssetIds)
{
	TArray<FPrimaryAssetId> id;
	GetAllIdOfType(TEXT("Blaster"), id);
	AssetIds = id;
}

void UPxiiInventoryComponent::GetAllArmbladeItemId(TArray<FPrimaryAssetId>& AssetIds)
{
	GetAllIdOfType(TEXT("Armblade"), AssetIds);
}

void UPxiiInventoryComponent::GetAllHeadItemId(TArray<FPrimaryAssetId>& AssetIds)
{
	GetAllIdOfType(TEXT("Head"), AssetIds);
}

void UPxiiInventoryComponent::GetAllUpperItemId(TArray<FPrimaryAssetId>& AssetIds)
{
	GetAllIdOfType(TEXT("Upper"), AssetIds);
}

void UPxiiInventoryComponent::GetAllLowerItemId(TArray<FPrimaryAssetId>& AssetIds)
{
	GetAllIdOfType(TEXT("Lower"), AssetIds);
}

void UPxiiInventoryComponent::GetAllTrinketItemId(TArray<FPrimaryAssetId>& AssetIds)
{
	GetAllIdOfType(TEXT("Trinkets"), AssetIds);
}

void UPxiiInventoryComponent::GetAllItemId(TArray<FPrimaryAssetId>& AssetIds)
{
	GetAllIdOfType(TEXT("Items"), AssetIds);
}

void UPxiiInventoryComponent::GetAllEquipmentId(TArray<FPrimaryAssetId>& AssetIds)
{
	TArray<FPrimaryAssetId> id;
	GetAllIdOfType(TEXT("Equipments"), id);
	AssetIds = id;
}

void UPxiiInventoryComponent::GetAssetID(UEquipmentItemData* ItemData)
{
	PXII_LOG(ELogCategory::Inventory, Log, TEXT("ID: %s"), *ItemData->GetPrimaryAssetId().ToString());

	TArray<FPrimaryAssetTypeInfo> TypeInfos;

	UAssetManager::Get().GetPrimaryAssetTypeInfoList(TypeInfos);

	for (const FPrimaryAssetTypeInfo& Info : TypeInfos)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("TYPE: %s | BaseClass: %s | Directories: %d"),
			*Info.PrimaryAssetType.ToString(),
			Info.GetAssetBaseClass()
				? *Info.GetAssetBaseClass()->GetPathName()
				: TEXT("NULL"),
			Info.GetDirectories().Num());
	}
}

TArray<UBaseItem*> UPxiiInventoryComponent::GetAllItemsOfSlotType(EEquipmentSlot SlotType)
{
	TArray<UBaseItem*> equipmentItems;

	TArray<UBaseItem*> allItems;
	GetSubsystem()->GetAllItems(allItems);
	for(const auto& item : allItems)
	{
		if(item->GetData()->ItemType != EItemType::Equipment)
		{
			continue;
		}

		UEquipmentItemData* data = Cast<UEquipmentItemData>(item->GetData());
		if(!data)
		{
			continue;
		}

		if(data->EquipmentSlot != SlotType)
		{
			continue;
		}
		
		equipmentItems.Add(item);
	}
	
	return equipmentItems;
}

void UPxiiInventoryComponent::GetAllIdOfType(FName InAssetType, TArray<FPrimaryAssetId>& OutAssetId)
{
	const FPrimaryAssetType Type(InAssetType);

	UE_LOG(LogTemp, Warning, TEXT("EquipmentItem: Type: %s Valid: %s"), *Type.ToString(), Type.IsValid() ? TEXT("YES") : TEXT("NO"));

	UAssetManager::Get().GetPrimaryAssetIdList( Type, OutAssetId	);

	UE_LOG(LogTemp, Warning, TEXT("EquipmentItem Count: %d"),OutAssetId.Num());

	for (const FPrimaryAssetId& Id : OutAssetId)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Equipment ID: %s"),
			*Id.ToString());
	}
}

TArray<FPrimaryAssetType> UPxiiInventoryComponent::GetAssetTypes()
{
	UAssetManager& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetType> Types;

	for (const FPrimaryAssetType& Type : Types)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Registered Primary Asset Type: %s"),
			*Type.ToString()
		);
	}
	
	return Types;
}

TArray<UBaseItemData> UPxiiInventoryComponent::GetItemData(TArray<FPrimaryAssetId> AssetIds)
{
	TArray<UBaseItemData> baseItemData;
	
	return baseItemData;
}

UPxiiInventorySubsystem* UPxiiInventoryComponent::GetSubsystem()
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
	
	subsystem = world->GetGameInstance()->GetSubsystem<UPxiiInventorySubsystem>();
	return subsystem;
}
