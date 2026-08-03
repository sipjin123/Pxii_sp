// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/DataObject/PxiiListDataObjectScalar.h"

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

float UPxiiListDataObjectScalar::GetCurrentValue() const
{
	if (DataDynamicGetter)
	{
		
	}
	
	return float();
}
