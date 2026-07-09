// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CheatMenu/PxiiCheatMenuScreen.h"
#include "Utility/PXIILogUtility.h"

void UPxiiCheatMenuScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// For reset action
	//if(ResetAction.IsNull())
	//{
	//	PXII_LOG(LogTemp, Warning, TEXT("Reset Action is null and is not assigned in table row."));
	//}
	//else
	//{
	//	ResetActionHandle = RegisterUIActionBinding(
	//	   FBindUIActionArgs(
	//		   ResetAction,
	//		   true,
	//		   FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
	//	   )
	//	);
	//}

	//RegisterUIActionBinding(
	//FBindUIActionArgs(
	//	ICommonInputModule::GetSettings().GetDefaultBackAction(),
	//	true,
	//	FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
	//)
	//);

	// Bind delegate
	TabListWidget_CheatTabs->OnTabSelected.AddDynamic(this, &ThisClass::OnTabSelected);
}

void UPxiiCheatMenuScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	//for(UListDataObject_Collection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionTabsCollections())
	//{
	//	if(!TabCollection)
	//	{
	//		continue;
	//	}

	//	const FName TabDataID = TabCollection->GetDataID();

	//	// Button already exists
	//	if(TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabDataID) != nullptr)
	//	{
	//		continue;
	//	}

	//	TabListWidget_OptionsTabs->RequestRegisterTab(TabDataID, TabCollection->GetDataDisplayName());
	//}
}

void UPxiiCheatMenuScreen::OnResetBoundActionTriggered()
{
}

void UPxiiCheatMenuScreen::OnBackBoundActionTriggered()
{
}

void UPxiiCheatMenuScreen::OnTabSelected(FName TabID)
{}
