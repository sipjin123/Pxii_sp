// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/ListEntries/PxiiListEntryBase.h"

#include "CommonInputSubsystem.h"
#include "Components/ListView.h"
#include "Utility/PXIILogUtility.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectCollection.h"

void UPxiiListEntryBase::NativeOnListEntryWidgetHovered(bool bWasHovered)
{
	BP_OnListEntryWidgetHovered(bWasHovered, IsListItemSelected());
}

FReply UPxiiListEntryBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
	
	if (CommonInputSubsystem && CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		if (UWidget* WidgetToFocus = BP_GamepadGetWidgetToFocus())
		{
			if (TSharedPtr<SWidget> SlateWidgetToFocus = WidgetToFocus->GetCachedWidget())
			{
				return FReply::Handled().SetUserFocus(SlateWidgetToFocus.ToSharedRef());
			}
		}
	}
	
	return Super::NativeOnFocusReceived(InGeometry,InFocusEvent);
}

void UPxiiListEntryBase::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	
	UPxiiListDataObjectBase* CastedDataObject = Cast<UPxiiListDataObjectBase>(ListItemObject);

	if(!CastedDataObject)
	{
		PXII_LOG(ELogCategory::UI, Warning, TEXT("[% s]: An item failed to cast to desired class"), *ThisClass::StaticClass()->GetName());
		return;
	}

	OnOwningListDataObjectSet(CastedDataObject);
}

void UPxiiListEntryBase::NativeOnEntryReleased()
{
	// This is for bug that cause double entry to be highlighted at the same time
	IUserObjectListEntry::NativeOnEntryReleased();
	
	if (UPxiiListDataObjectBase* Data = Cast<UPxiiListDataObjectBase>(GetListItem()))
	{
		Data->OnListDataModified.RemoveAll(this);
	}
	
	NativeOnListEntryWidgetHovered(false);
}

void UPxiiListEntryBase::OnOwningListDataObjectSet(UPxiiListDataObjectBase* InOwningListDataObject)
{
	if (Cast<UPxiiListDataObjectCollection>(InOwningListDataObject))
	{
		this->SetVisibility(ESlateVisibility::HitTestInvisible);	
	}
	else
	{
		this->SetVisibility(ESlateVisibility::Visible);
	}
	
	if(TextBlock_EntryDisplayName)
	{
		TextBlock_EntryDisplayName->SetText(InOwningListDataObject->GetDataDisplayName());
	}

	if(!InOwningListDataObject->OnListDataModified.IsBoundToObject(this))
	{
		InOwningListDataObject->OnListDataModified.AddUObject(this, &ThisClass::OnOwningListDataObjectModified);
	}
}

void UPxiiListEntryBase::OnOwningListDataObjectModified(UPxiiListDataObjectBase* InModifiedListDataObject, 
	EListDataModifyType InModifyType)
{

}

void UPxiiListEntryBase::SelectThisEntryWidget()
{
	UListView* OwningListView = Cast<UListView>(GetOwningListView());
	
	if (!OwningListView)
	{
		return;
	}
	
	OwningListView->SetSelectedItem(GetListItem());
}
