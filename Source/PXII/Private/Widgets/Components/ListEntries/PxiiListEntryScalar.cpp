// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/ListEntries/PxiiListEntryScalar.h"
#include "Utility/PXIILogUtility.h"

void UPxiiListEntryScalar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (AnalogSlider_Slider->OnValueChanged.IsBound())
	{
		AnalogSlider_Slider->OnValueChanged.Clear();
	}
	
	AnalogSlider_Slider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::OnSliderValueChange);
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
	//NumericTextBlock_Value->SetCurrentValue(CachedOwningListDataObjectScalar->GetCurrentValue(CachedOwningListDataObjectScalar->GetDataID()));
	
	AnalogSlider_Slider->SetMinValue(CachedOwningListDataObjectScalar->GetDisplayValueRange().GetLowerBoundValue());
	AnalogSlider_Slider->SetMaxValue(CachedOwningListDataObjectScalar->GetDisplayValueRange().GetUpperBoundValue());
	AnalogSlider_Slider->SetStepSize(CachedOwningListDataObjectScalar->GetSliderStepSize());
	
	//AnalogSlider_Slider->SetValue(CachedOwningListDataObjectScalar->GetCurrentValue(CachedOwningListDataObjectScalar->GetDataID()));
	OnOwningListDataObjectModified(CachedOwningListDataObjectScalar, EListDataModifyType::DirectlyModified);
}

void UPxiiListEntryScalar::OnOwningListDataObjectModified(UPxiiListDataObjectBase* InModifiedListDataObject,
	EListDataModifyType InModifyType)
{
	if (CachedOwningListDataObjectScalar)
	{
		NumericTextBlock_Value->SetCurrentValue(CachedOwningListDataObjectScalar->GetCurrentValue(CachedOwningListDataObjectScalar->GetDataID()));
		AnalogSlider_Slider->SetValue(CachedOwningListDataObjectScalar->GetCurrentValue(CachedOwningListDataObjectScalar->GetDataID()));
	}
}

void UPxiiListEntryScalar::OnSliderValueChange(float Value)
{
	if (CachedOwningListDataObjectScalar)
	{
		CachedOwningListDataObjectScalar->SetCurrentValue(CachedOwningListDataObjectScalar->GetDataID(), Value);
	}
}
