// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OptionsMenu/PxiiOptionsMenuScreen.h"

#include "Editor/WidgetCompilerLog.h"
#include "Input/CommonUIInputTypes.h"
#include "Utility/PXIILogUtility.h"

void UPxiiOptionsMenuScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (ResetAction.IsNull() || OptionsBackAction.IsNull())
	{
		PXII_LOG(ELogCategory::UI, Warning, TEXT("Reset Action or Back Action is null and is not assigned in table row."));
	}
	else
	{
		ResetActionHandle = RegisterUIActionBinding(
		   FBindUIActionArgs(
			   ResetAction,
			   true,
			   FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
		   )
		);
		
		BackActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				OptionsBackAction,
				true,
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
			)
		);
	}
	
	// Bind delegate
	TabListWidget_OptionsTabs->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnTabSelected);
	
	ListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnListViewItemHovered);
	ListView_OptionsList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListViewItemSelected);
}

void UPxiiOptionsMenuScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	for (UPxiiListDataObjectCollection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredTabsCollections())
	{
		if (!TabCollection)
		{
			continue;
		}
		
		const FName TabID = TabCollection->GetDataID();
		
		// If the tab already exists
		if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID) != nullptr)
		{
			continue;
		}
		
		TabListWidget_OptionsTabs->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
	}
}

void UPxiiOptionsMenuScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	UPxiiGameUserSettings::Get()->ApplySettings(true);
}

UWidget* UPxiiOptionsMenuScreen::NativeGetDesiredFocusTarget() const
{
	// For regaining focus back after reactivate this option screen widget
	if (UObject* SelectedItem = ListView_OptionsList->GetSelectedItem())
	{
		if (UUserWidget* SelectedEntryWidget = ListView_OptionsList->GetEntryWidgetFromItem(SelectedItem))
		{
			return SelectedEntryWidget;
		}
	}
	
	return Super::NativeGetDesiredFocusTarget();
}

UPxiiOptionsDataRegistry* UPxiiOptionsMenuScreen::GetOrCreateDataRegistry()
{
	if (!CreatedOwningDataRegistry)
	{
		CreatedOwningDataRegistry = NewObject<UPxiiOptionsDataRegistry>();
		CreatedOwningDataRegistry->SetOptionsData(OptionsData);
		CreatedOwningDataRegistry->InitDataRegistry(GetOwningLocalPlayer());
	}
	
	if (!CreatedOwningDataRegistry)
	{
		PXII_LOG(ELogCategory::UI, Error, TEXT("[%s]: Data registry for options menu is not valid"), *ThisClass::StaticClass()->GetName());
		return nullptr;
	}
	
	return CreatedOwningDataRegistry;
}

void UPxiiOptionsMenuScreen::OnResetBoundActionTriggered()
{
	// Reset Settings
	PxiiLog::Print(ThisClass::StaticClass()->GetName(), TEXT("Reset Triggered"));
	if (ResettableDataArray.IsEmpty())
	{
		return; 
		// but should not happen when it is empty, cuz the button will be hide from being triggered
		// this is for double checking
	}
	
	//TODO: Confirmation screen
	bIsResettingData = true;
	bool bHasDataFailedToReset = false;
	for (UPxiiListDataObjectBase* DataToReset : ResettableDataArray)
	{
		if (!DataToReset)
		{
			continue;
		}
		
		if (DataToReset->TryResetBackToDefaultValue())
		{
			PxiiLog::Print(ThisClass::StaticClass()->GetName(), FString::Printf(TEXT("Reset %s"), *DataToReset->GetDataDisplayName().ToString()));
		}
		else
		{
			bHasDataFailedToReset = true;
			PxiiLog::Print(ThisClass::StaticClass()->GetName(), FString::Printf(TEXT("Failed to reset %s"), *DataToReset->GetDataDisplayName().ToString()));
		}
	}
	
	if (!bHasDataFailedToReset)
	{
		ResettableDataArray.Empty();
		RemoveActionBinding(ResetActionHandle);
	}
	
	bIsResettingData = false;
}

void UPxiiOptionsMenuScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

FString UPxiiOptionsMenuScreen::TryGetEntryWidgetClassName(UObject* InOwningListItem) const
{
	UUserWidget* FoundEntryWidget = ListView_OptionsList->GetEntryWidgetFromItem(InOwningListItem);
	
	if (FoundEntryWidget)
	{
		return FoundEntryWidget->GetClass()->GetName();
	}
	
	return TEXT("Entry widget not valid");
}

#if WITH_EDITOR	
void UPxiiOptionsMenuScreen::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	if (!OptionsData)
	{
		CompileLog.Error(FText::FromString(TEXT("Options data is not assigned a valid data asset")));
	}
}
#endif

void UPxiiOptionsMenuScreen::OnTabSelected(FName TabID)
{
	OptionsDetailsView->ClearDetailsInfo();
	
	TArray<UPxiiListDataObjectBase*> FoundListSourceItems = GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabID);
	
	ListView_OptionsList->SetListItems(FoundListSourceItems);
	ListView_OptionsList->RequestRefresh();
	
	if (ListView_OptionsList->GetNumItems() > 0)
	{
		ListView_OptionsList->NavigateToIndex(0);
		ListView_OptionsList->SetSelectedIndex(0);
	}
	
	// Check if there is data to be reset, if yes show reset button and vice versa
	ResettableDataArray.Empty();
	
	for (UPxiiListDataObjectBase* FoundListSourceItem : FoundListSourceItems)
	{
		if (!FoundListSourceItem)
		{
			continue;
		}
		
		if (!FoundListSourceItem->OnListDataModified.IsBoundToObject(this))
		{
			FoundListSourceItem->OnListDataModified.AddUObject(this, &ThisClass::OnListViewDataObjectModified);	
		}
		
		if (FoundListSourceItem->CanResetBackToDefaultValue())
		{
			ResettableDataArray.AddUnique(FoundListSourceItem);
		}
	}
	
	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
	else
	{
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
}

void UPxiiOptionsMenuScreen::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
	if (!InHoveredItem)
	{
		PXII_LOG(ELogCategory::UI, Error, TEXT("[%s]: Hovered item is not valid"), *ThisClass::StaticClass()->GetName());
		return;
	}
	
	UPxiiListEntryBase* HoveredWidget = ListView_OptionsList->GetEntryWidgetFromItem<UPxiiListEntryBase>(InHoveredItem);
	
	if (!HoveredWidget)
	{
		PXII_LOG(ELogCategory::UI, Error, TEXT("[%s]: Hovered entry widget is not valid"), *ThisClass::StaticClass()->GetName());
		return;
	}
	
	HoveredWidget->NativeOnListEntryWidgetHovered(bWasHovered);
	
	if (bWasHovered)
	{
		UPxiiListDataObjectBase* DO = Cast<UPxiiListDataObjectBase>(InHoveredItem);
		OptionsDetailsView->UpdateDetailsInfo(DO, TryGetEntryWidgetClassName(InHoveredItem));
	}
	else
	{
		if (UPxiiListDataObjectBase* SelectedItem = ListView_OptionsList->GetSelectedItem<UPxiiListDataObjectBase>())
		{
			OptionsDetailsView->UpdateDetailsInfo(SelectedItem, TryGetEntryWidgetClassName(SelectedItem));
		}
	}
}

void UPxiiOptionsMenuScreen::OnListViewItemSelected(UObject* InSelectedItem)
{
	if (!InSelectedItem)
	{
		return;
	}
	
	UPxiiListDataObjectBase* DO = Cast<UPxiiListDataObjectBase>(InSelectedItem);
	OptionsDetailsView->UpdateDetailsInfo(DO, TryGetEntryWidgetClassName(InSelectedItem));
}

void UPxiiOptionsMenuScreen::OnListViewDataObjectModified(UPxiiListDataObjectBase* InModifiedDataObject,
	EListDataModifyType ModifiedReason)
{
	if (!InModifiedDataObject || bIsResettingData)
	{
		return;
	}
	
	if (InModifiedDataObject->CanResetBackToDefaultValue())
	{
		ResettableDataArray.AddUnique(InModifiedDataObject);
		
		if (!GetActionBindings().Contains(ResetActionHandle))
		{
			AddActionBinding(ResetActionHandle);
		}
	}
	else
	{
		if (ResettableDataArray.Contains(InModifiedDataObject))
		{
			ResettableDataArray.Remove(InModifiedDataObject);
		}
	}
	
	if (ResettableDataArray.IsEmpty())
	{
		RemoveActionBinding(ResetActionHandle);
	}
}
