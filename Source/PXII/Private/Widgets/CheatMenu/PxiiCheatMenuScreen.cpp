// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CheatMenu/PxiiCheatMenuScreen.h"
#include "Utility/PXIILogUtility.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectCollection.h"
#include "Utility/PXIILogUtility.h"

void UPxiiCheatMenuScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// For reset action
	if(ResetAction.IsNull())
	{
		PXII_LOG(ELogCategory::UI, Warning, TEXT("Reset Action is null and is not assigned in table row."));
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
	}

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
		)
	);

	// Bind delegate
	TabListWidget_CheatTabs->OnTabSelected.AddDynamic(this, &ThisClass::OnTabSelected);
}

void UPxiiCheatMenuScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for(UPxiiListDataObjectCollection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredTabsCollections())
	{
		if(!TabCollection)
		{
			continue;
		}

		const FName TabDataID = TabCollection->GetDataID();

		// Button already exists
		if(TabListWidget_CheatTabs->GetTabButtonBaseByID(TabDataID) != nullptr)
		{
			continue;
		}

		TabListWidget_CheatTabs->RequestRegisterTab(TabDataID, TabCollection->GetDataDisplayName());
	}
}

UPxiiCheatDataRegistry* UPxiiCheatMenuScreen::GetOrCreateDataRegistry()
{
	if(!CreatedOwningDataRegistry)
	{
		CreatedOwningDataRegistry = NewObject<UPxiiCheatDataRegistry>();
		CreatedOwningDataRegistry->InitDataRegistry(GetOwningLocalPlayer());
	}

	if(!CreatedOwningDataRegistry)
	{
		PXII_LOG(ELogCategory::UI, Error, TEXT("[%s]: Data registry for cheat menu is not valid"), *ThisClass::StaticClass()->GetName());
		return nullptr;
	}

	return CreatedOwningDataRegistry;
}

void UPxiiCheatMenuScreen::OnResetBoundActionTriggered()
{
	// reset settings
	PXII_LOG(ELogCategory::UI, Log, TEXT("Reset action triggered"));
}

void UPxiiCheatMenuScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}
