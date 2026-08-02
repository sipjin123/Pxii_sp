// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Components/DataObject/PxiiListDataObjectBase.h"

void UPxiiListDataObjectBase::InitDataObject()
{
	OnDataObjectInitialized();
}

void UPxiiListDataObjectBase::OnDataObjectInitialized()
{

}

void UPxiiListDataObjectBase::NotifyListDataModified(UPxiiListDataObjectBase* InModifiedListData, EListDataModifyType InModifyReason)
{
	OnListDataModified.Broadcast(InModifiedListData, InModifyReason);
}
