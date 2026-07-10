// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/DataRegistries/PxiiCheatDataRegistry.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectCollection.h"
#include "Utility/PXIILogUtility.h"

void UPxiiCheatDataRegistry::InitDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitCheatTab1();
	InitCheatTab2();
	InitCheatTab3();
}

TArray<UPxiiListDataObjectBase*> UPxiiCheatDataRegistry::GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID)
{
	UPxiiListDataObjectCollection* const* FoundTabCollectionPtr = RegisteredTabsCollections.FindByPredicate(
		[InSelectedTabID](UPxiiListDataObjectCollection* AvailableTabCollection)->bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);

	if(!FoundTabCollectionPtr)
	{
		PXII_LOG(ELogCategory::UI, Error, TEXT("No valid tab found under %s"), *InSelectedTabID.ToString());
	}

	UPxiiListDataObjectCollection* FoundTabCollection = *FoundTabCollectionPtr;

	return FoundTabCollection->GetAllChildListData();
}

void UPxiiCheatDataRegistry::InitCheatTab1()
{
	UPxiiListDataObjectCollection* Tab1Collection = NewObject<UPxiiListDataObjectCollection>();
	Tab1Collection->SetDataID(FName("Tab1"));
	Tab1Collection->SetDataDisplayName(FText::FromString(TEXT("Tab 1")));

	RegisteredTabsCollections.Add(Tab1Collection);
}

void UPxiiCheatDataRegistry::InitCheatTab2()
{
	UPxiiListDataObjectCollection* Tab2Collection = NewObject<UPxiiListDataObjectCollection>();
	Tab2Collection->SetDataID(FName("Tab2"));
	Tab2Collection->SetDataDisplayName(FText::FromString(TEXT("Tab 2")));

	RegisteredTabsCollections.Add(Tab2Collection);
}

void UPxiiCheatDataRegistry::InitCheatTab3()
{
	UPxiiListDataObjectCollection* Tab3Collection = NewObject<UPxiiListDataObjectCollection>();
	Tab3Collection->SetDataID(FName("Tab3"));
	Tab3Collection->SetDataDisplayName(FText::FromString(TEXT("Tab 3")));

	RegisteredTabsCollections.Add(Tab3Collection);
}
