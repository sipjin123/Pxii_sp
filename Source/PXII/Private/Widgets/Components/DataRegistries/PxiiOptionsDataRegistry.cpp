// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/DataRegistries/PxiiOptionsDataRegistry.h"
#include "Utility/PXIILogUtility.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectScalar.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectString.h"

void UPxiiOptionsDataRegistry::InitDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	ConstructTabs();
}

TArray<UPxiiListDataObjectBase*> UPxiiOptionsDataRegistry::GetListSourceItemsBySelectedTabID(
	const FName& InSelectedTabID)
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

	TArray<UPxiiListDataObjectBase*> AllChildListItems;
	
	for (UPxiiListDataObjectBase* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData)
		{
			continue;
		}
		
		AllChildListItems.Add(ChildListData);
		
		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(ChildListData, AllChildListItems);
		}
	}
	
	return AllChildListItems;
}

void UPxiiOptionsDataRegistry::ConstructTabs()
{
	if (!OptionsData)
	{
		PXII_LOG(ELogCategory::UI, Error, TEXT("[%s]: Invalid options data"), *ThisClass::StaticClass()->GetName());
		return;
	}
	
	if (OptionsData->TabsToConstruct.IsEmpty())
	{
		PXII_LOG(ELogCategory::UI, Error, TEXT("[%s]: No options tabs to construct"), *ThisClass::StaticClass()->GetName());
		return;
	}
	
	for (const FTabsEntry& Tab : OptionsData->TabsToConstruct)
	{
		UPxiiListDataObjectCollection* TabCollection = NewObject<UPxiiListDataObjectCollection>();
		TabCollection->SetDataID(FName(Tab.TabName.ToString()));
		TabCollection->SetDataDisplayName(Tab.TabName);
	
		// sub
		if (Tab.bSubCategory)
		{
			if (!Tab.SubDataToConstruct.IsEmpty())
			{
				for (const FSubOptionsDataEntry& SubOptionsEntry : Tab.SubDataToConstruct)
				{
					UPxiiListDataObjectCollection* SubCollection = NewObject<UPxiiListDataObjectCollection>();
					SubCollection->SetDataID(FName(SubOptionsEntry.CategoryID.ToString()));
					SubCollection->SetDataDisplayName(SubOptionsEntry.CategoryDisplayName);
			
					TabCollection->AddChildListData(SubCollection);
					
					ConstructDataObjects(SubCollection, SubOptionsEntry.DataToConstruct);
				}
			}
		}
		else
		{
			ConstructDataObjects(TabCollection, Tab.DataToConstruct);
		}
	
		RegisteredTabsCollections.AddUnique(TabCollection);	
	}
}

void UPxiiOptionsDataRegistry::ConstructDataObjects(UPxiiListDataObjectCollection* CollectionToAdd, const TArray<FOptionsDataEntry>& DataEntries)
{
	// not sub
	for (const FOptionsDataEntry& OptionsEntry : DataEntries)
	{
		if (OptionsEntry.Type == EOptionsDataType::String)
		{
			if (OptionsEntry.Data.GetScriptStruct() == FStringData::StaticStruct())
			{
				if (const FStringData* Data = OptionsEntry.Data.GetPtr<FStringData>())
				{
					UPxiiListDataObjectString* DO = NewObject<UPxiiListDataObjectString>();
					DO->SetDataID(FName(Data->DataID.ToString()));
					DO->SetDataDisplayName(Data->DataDisplayName);
					
					DO->SetDescriptionRichText(Data->DescriptionRichText);
					
					for (const FOptionsMap& Option : Data->OptionsSet)
					{
						DO->AddDynamicOptions(Option.Value.ToString(), Option.DisplayName);
					}
					
					CollectionToAdd->AddChildListData(DO);
				}
			}
		}
		else if (OptionsEntry.Type == EOptionsDataType::Boolean)
		{
			
		}
		else if (OptionsEntry.Type == EOptionsDataType::Slider)
		{
			if (OptionsEntry.Data.GetScriptStruct() == FSliderData::StaticStruct())
			{
				if (const FSliderData* Data = OptionsEntry.Data.GetPtr<FSliderData>())
				{
					UPxiiListDataObjectScalar* DO = NewObject<UPxiiListDataObjectScalar>();
					DO->SetDataID(FName(Data->DataID.ToString()));
					DO->SetDataDisplayName(Data->DataDisplayName);
						
					DO->SetDescriptionRichText(Data->DescriptionRichText);
					DO->SetDisplayValueRange(TRange<float>(Data->DisplayValueRange.X, Data->DisplayValueRange.Y));
					DO->SetOutputValueRange(TRange<float>(Data->OutputValueRange.X, Data->OutputValueRange.Y));
					DO->SetSliderStepSize(Data->SliderStepSize);
					DO->SetDefaultValueFromString(LexToString(1.f));
					DO->SetDisplayNumericType(Data->DisplayNumericType);
						
					if (!Data->bShowDecimal)
					{
						DO->SetNumericFormattingOptions(UPxiiListDataObjectScalar::NoDecimal());
					}
					else
					{
						DO->SetNumericFormattingOptions(UPxiiListDataObjectScalar::WithDecimal(Data->FractionalDigits));
					}
					
					CollectionToAdd->AddChildListData(DO);
				}
			}
		}
	}
}

void UPxiiOptionsDataRegistry::FindChildListDataRecursively(UPxiiListDataObjectBase* InParentData,
	TArray<UPxiiListDataObjectBase*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
	{
		return;
	}
	
	for (UPxiiListDataObjectBase* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData)
		{
			continue;
		}
		
		OutFoundChildListData.Add(SubChildListData);
		
		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, OutFoundChildListData);
		}
	}
}
