// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/DataRegistries/PxiiCheatDataRegistry.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectCollection.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectString.h"
#include "Utility/PXIILogUtility.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectScalar.h"

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
		PXII_LOG(ELogCategory::UI, Error, TEXT("[%s]: No valid tab found under %s"), *ThisClass::StaticClass()->GetName(), *InSelectedTabID.ToString());
	}

	UPxiiListDataObjectCollection* FoundTabCollection = *FoundTabCollectionPtr;

	return FoundTabCollection->GetAllChildListData();
}

void UPxiiCheatDataRegistry::InitCheatTab1()
{
	UPxiiListDataObjectCollection* Tab1Collection = NewObject<UPxiiListDataObjectCollection>();
	Tab1Collection->SetDataID(FName("Tab1"));
	Tab1Collection->SetDataDisplayName(FText::FromString(TEXT("Tab 1")));

	// Construct data object
	{
		{
			UPxiiListDataObjectString* DO = NewObject<UPxiiListDataObjectString>();
			DO->SetDataID(FName("String1"));
			DO->SetDataDisplayName(FText::FromString("String1"));

			DO->AddDynamicOptions(TEXT("1"), FText::FromString("1"));
			DO->AddDynamicOptions(TEXT("2"), FText::FromString("2"));
			DO->AddDynamicOptions(TEXT("3"), FText::FromString("3"));
			DO->AddDynamicOptions(TEXT("4"), FText::FromString("4"));

			Tab1Collection->AddChildListData(DO);
		}

		{
			UPxiiListDataObjectString* DO = NewObject<UPxiiListDataObjectString>();
			DO->SetDataID(FName("String2"));
			DO->SetDataDisplayName(FText::FromString("String2"));

			DO->AddDynamicOptions(TEXT("1"), FText::FromString("1"));
			DO->AddDynamicOptions(TEXT("2"), FText::FromString("2"));
			DO->AddDynamicOptions(TEXT("3"), FText::FromString("3"));
			DO->AddDynamicOptions(TEXT("4"), FText::FromString("4"));
			
			Tab1Collection->AddChildListData(DO);
		}
		
		{
			UPxiiListDataObjectScalar* DO = NewObject<UPxiiListDataObjectScalar>();
			DO->SetDataID(FName("Slider1"));
			DO->SetDataDisplayName(FText::FromString("Slider1"));
			
			DO->SetDescriptionRichText(FText::FromString("This is description for slider 1"));
			DO->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			DO->SetOutputValueRange(TRange<float>(0.f, 2.f));
			DO->SetSliderStepSize(0.01f);
			DO->SetDefaultValueFromString(LexToString(1.f));
			DO->SetDisplayNumericType(ECommonNumericType::Percentage);
			DO->SetNumericFormattingOptions(UPxiiListDataObjectScalar::NoDecimal());
			
			//TODO: Set data dynamic getter and setter
			
			Tab1Collection->AddChildListData(DO);
		}
	}

	RegisteredTabsCollections.Add(Tab1Collection);
}

void UPxiiCheatDataRegistry::InitCheatTab2()
{
	UPxiiListDataObjectCollection* Tab2Collection = NewObject<UPxiiListDataObjectCollection>();
	Tab2Collection->SetDataID(FName("Tab2"));
	Tab2Collection->SetDataDisplayName(FText::FromString(TEXT("Tab 2")));

	{
		UPxiiListDataObjectString* DO = NewObject<UPxiiListDataObjectString>();
		DO->SetDataID(FName("String3"));
		DO->SetDataDisplayName(FText::FromString("String3"));

		Tab2Collection->AddChildListData(DO);
	}

	{
		UPxiiListDataObjectString* DO = NewObject<UPxiiListDataObjectString>();
		DO->SetDataID(FName("String4"));
		DO->SetDataDisplayName(FText::FromString("String4"));

		Tab2Collection->AddChildListData(DO);
	}

	RegisteredTabsCollections.Add(Tab2Collection);
}

void UPxiiCheatDataRegistry::InitCheatTab3()
{
	UPxiiListDataObjectCollection* Tab3Collection = NewObject<UPxiiListDataObjectCollection>();
	Tab3Collection->SetDataID(FName("Tab3"));
	Tab3Collection->SetDataDisplayName(FText::FromString(TEXT("Tab 3")));

	{
		UPxiiListDataObjectString* DO = NewObject<UPxiiListDataObjectString>();
		DO->SetDataID(FName("String5"));
		DO->SetDataDisplayName(FText::FromString("String5"));

		Tab3Collection->AddChildListData(DO);
	}

	{
		UPxiiListDataObjectString* DO = NewObject<UPxiiListDataObjectString>();
		DO->SetDataID(FName("String6"));
		DO->SetDataDisplayName(FText::FromString("String6"));

		Tab3Collection->AddChildListData(DO);
	}

	RegisteredTabsCollections.Add(Tab3Collection);
}
