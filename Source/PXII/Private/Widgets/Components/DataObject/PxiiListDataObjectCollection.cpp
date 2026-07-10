// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Components/DataObject/PxiiListDataObjectCollection.h"

void UPxiiListDataObjectCollection::AddChildListData(UPxiiListDataObjectBase* InChildListData)
{
	//Notify the child list data to init itself
	InChildListData->InitDataObject();

	//Set the child list data's parent to this collection
	InChildListData->SetParentListData(this);

	ChildListDataArray.Add(InChildListData);
}

TArray<UPxiiListDataObjectBase*> UPxiiListDataObjectCollection::GetAllChildListData() const
{
	return ChildListDataArray;
}

bool UPxiiListDataObjectCollection::HasAnyChildListData() const
{
	return !ChildListDataArray.IsEmpty();
}
