// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/ListEntries/PxiiListEntryString.h"
#include "Utility/PXIILogUtility.h"

void UPxiiListEntryString::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Button_Previous->OnClicked().AddUObject(this, &ThisClass::OnPreviousButtonClicked);
	Button_Next->OnClicked().AddUObject(this, &ThisClass::OnNextButtonClicked);
}

void UPxiiListEntryString::OnOwningListDataObjectSet(UPxiiListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
	
	CachedOwningListDataObjectString = Cast<UPxiiListDataObjectString>(InOwningListDataObject);
	
	if (!IsValid(CachedOwningListDataObjectString))
	{
		PXII_LOG(ELogCategory::UI, Warning, TEXT("[% s]: Failed to cast to list data object string"), *ThisClass::StaticClass()->GetName());
	}
	
	Rotator_Options->PopulateTextLabels(CachedOwningListDataObjectString->GetAvailableDisplayTexts());
	Rotator_Options->SetSelectedOptionByText(CachedOwningListDataObjectString->GetCurrentDisplayText());
}

void UPxiiListEntryString::OnOwningListDataObjectModified(UPxiiListDataObjectBase* InModifiedListDataObject,
	EListDataModifyType InModifyType)
{
	if (CachedOwningListDataObjectString)
	{
		Rotator_Options->SetSelectedOptionByText(CachedOwningListDataObjectString->GetCurrentDisplayText());
	}
}

void UPxiiListEntryString::OnPreviousButtonClicked()
{
	if (CachedOwningListDataObjectString)
	{
		CachedOwningListDataObjectString->PreviousOption();
	}
}

void UPxiiListEntryString::OnNextButtonClicked()
{
	if (CachedOwningListDataObjectString)
	{
		CachedOwningListDataObjectString->NextOption();
	}
}
