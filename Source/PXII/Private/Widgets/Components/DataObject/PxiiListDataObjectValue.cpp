// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Components/DataObject/PxiiListDataObjectValue.h"

void UPxiiListDataObjectValue::SetDataDynamicGetter(const TSharedPtr<FPxiiDataInteractionUtility>& InDynamicGetter)
{
	DataDynamicGetter = InDynamicGetter;
}

void UPxiiListDataObjectValue::SetDataDynamicSetter(const TSharedPtr<FPxiiDataInteractionUtility>& InDynamicSetter)
{
	DataDynamicSetter = InDynamicSetter;
}
