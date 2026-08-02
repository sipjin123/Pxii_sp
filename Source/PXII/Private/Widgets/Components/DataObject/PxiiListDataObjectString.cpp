// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Components/DataObject/PxiiListDataObjectString.h"
#include "Utility/PXIILogUtility.h"

void UPxiiListDataObjectString::AddDynamicOptions(const FString& InStringValue, const FText& InDisplayText)
{
	AvailableStringValues.Add(InStringValue);
	AvailableDisplayTexts.Add(InDisplayText);
}

void UPxiiListDataObjectString::NextOption()
{
	if (AvailableStringValues.IsEmpty() || AvailableDisplayTexts.IsEmpty())
	{
		return;
	}
	
	const int32 CurrentStringIndex = AvailableStringValues.IndexOfByKey(CurrentStringValue);
	const int32 NextStringIndex = CurrentStringIndex + 1;
	
	if (AvailableStringValues.IsValidIndex(NextStringIndex))
	{
		CurrentStringValue = AvailableStringValues[NextStringIndex];
	}
	else
	{
		CurrentStringValue = AvailableStringValues[0];
	}
	
	TrySetDisplayText(CurrentStringValue);
	NotifyListDataModified(this);
}

void UPxiiListDataObjectString::PreviousOption()
{
	if (AvailableStringValues.IsEmpty() || AvailableDisplayTexts.IsEmpty())
	{
		return;
	}
	
	const int32 CurrentStringIndex = AvailableStringValues.IndexOfByKey(CurrentStringValue);
	const int32 PreviousStringIndex = CurrentStringIndex - 1;
	
	if (AvailableStringValues.IsValidIndex(PreviousStringIndex))
	{
		CurrentStringValue = AvailableStringValues[PreviousStringIndex];
	}
	else
	{
		CurrentStringValue = AvailableStringValues.Last();
	}
	
	TrySetDisplayText(CurrentStringValue);
	NotifyListDataModified(this);
}

void UPxiiListDataObjectString::OnDataObjectInitialized()
{
	if (!AvailableStringValues.IsEmpty())
	{
		CurrentStringValue = AvailableStringValues[0];
	}
	
	//TODO: read the saved value to set as current value
	
	if (!TrySetDisplayText(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString("Invalid Display Text");
	}
}

bool UPxiiListDataObjectString::TrySetDisplayText(const FString& InStringValue)
{
	const int Index = AvailableStringValues.IndexOfByKey(InStringValue);
	
	if (AvailableDisplayTexts.IsValidIndex(Index))
	{
		CurrentDisplayText = AvailableDisplayTexts[Index];
		
		return true;
	}
	
	return false;
}
