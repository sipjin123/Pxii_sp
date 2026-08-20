// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/DataObject/PxiiListDataObjectScalar.h"

#include "Utility/PXIILogUtility.h"

FCommonNumberFormattingOptions UPxiiListDataObjectScalar::NoDecimal()
{
	FCommonNumberFormattingOptions FormattingOptions;
	FormattingOptions.MaximumFractionalDigits = 0;
	
	return FormattingOptions;
}

FCommonNumberFormattingOptions UPxiiListDataObjectScalar::WithDecimal(int32 InFractionalDigit)
{
	FCommonNumberFormattingOptions FormattingOptions;
	FormattingOptions.MaximumFractionalDigits = InFractionalDigit;
	
	return FormattingOptions;
}

float UPxiiListDataObjectScalar::GetCurrentValue(FName InSettingsID) const
{
	if (DataDynamicGetter)
	{
		return FMath::GetMappedRangeValueClamped(
			OutputValueRange, 
			DisplayValueRange, 
			StringToFloat(DataDynamicGetter->GetSavedValueAsString(InSettingsID))
		);
	}
	PXII_LOG(ELogCategory::UI, Warning, TEXT("[% s]: Invalid data dynamic getter"), *ThisClass::StaticClass()->GetName());
	return 0.f;
}

void UPxiiListDataObjectScalar::SetCurrentValue(FName InSettingsID, float InValue)
{
	if (DataDynamicSetter)
	{
		const float ClampedValue = FMath::GetMappedRangeValueClamped(
			DisplayValueRange,
			OutputValueRange,
			InValue
		);
		DataDynamicSetter->SetValueToSaveFromString(InSettingsID, LexToString(ClampedValue));
		
		NotifyListDataModified(this);
	}
}

float UPxiiListDataObjectScalar::StringToFloat(const FString& InString) const
{
	float ConvertedValue = 0.f;
	LexFromString(ConvertedValue, *InString);
	
	return ConvertedValue;
}
