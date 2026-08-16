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
		ListView_OptionsList->ClearSelection();
		ListView_OptionsList->SetSelectedIndex(0);
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
	
	UPxiiListEntryBase* SelectedWidget = ListView_OptionsList->GetEntryWidgetFromItem<UPxiiListEntryBase>(InSelectedItem);
}
