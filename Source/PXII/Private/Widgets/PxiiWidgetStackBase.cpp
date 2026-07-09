// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/PxiiWidgetStackBase.h"
#include "Utility/PXIILogUtility.h"

UCommonActivatableWidgetContainerBase* UPxiiWidgetStackBase::FindStackByTag(const FGameplayTag& InStackTag) const
{
	if (RegisteredWidgetStackMap.Contains(InStackTag))
	{
		return RegisteredWidgetStackMap.FindRef(InStackTag);
	}

	PXII_LOG(ELogCategory::UI, Warning, TEXT("[%s]: Widget %s not found"), *GetClass()->GetName(), *InStackTag.ToString());

	return nullptr;
}

void UPxiiWidgetStackBase::RegisterWidgetStack(UPARAM(meta = (Categories = "Pxii.UI.WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InWidgetStack)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InWidgetStack);
		}
	}
}