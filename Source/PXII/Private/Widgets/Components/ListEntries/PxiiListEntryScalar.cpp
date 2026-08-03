// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/ListEntries/PxiiListEntryScalar.h"
#include "Utility/PXIILogUtility.h"

void UPxiiListEntryScalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPxiiListEntryScalar::OnOwningListDataObjectSet(UPxiiListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
	
	CachedOwningListDataObjectScalar = Cast<UPxiiListDataObjectScalar>(InOwningListDataObject);
	
	if (!IsValid(CachedOwningListDataObjectScalar))
	{
		PXII_LOG(ELogCategory::UI, Warning, TEXT("[% s]: Failed to cast to list data object scalar"), *ThisClass::StaticClass()->GetName());
	}
	
	NumericTextBlock_Value->SetNumericType(CachedOwningListDataObjectScalar->GetDisplayNumericType());
	NumericTextBlock_Value->FormattingSpecification = CachedOwningListDataObjectScalar->GetNumericFormattingOptions();
	NumericTextBlock_Value->SetCurrentValue(CachedOwningListDataObjectScalar->GetCurrentValue());
}

void UPxiiListEntryScalar::OnOwningListDataObjectModified(UPxiiListDataObjectBase* InModifiedListDataObject,
	EListDataModifyType InModifyType)
{
	
}
