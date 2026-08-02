// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/ListEntries/PxiiListEntryBase.h"
#include "Utility/PXIILogUtility.h"

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

void UPxiiListEntryBase::OnOwningListDataObjectSet(UPxiiListDataObjectBase* InOwningListDataObject)
{
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
