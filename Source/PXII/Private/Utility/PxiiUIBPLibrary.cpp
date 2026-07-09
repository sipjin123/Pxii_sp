// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/PxiiUIBPLibrary.h"
#include "Settings/UIDeveloperSettings.h"
#include "Utility/PXIILogUtility.h"

TSoftClassPtr<UPxiiActivatableWidget> UPxiiUIBPLibrary::GetWidgetSoftClassByTag(UPARAM(meta = (Categories = "Pxii.UI.Widget")) const FGameplayTag InWidgetTag)
{
	const UUIDeveloperSettings* UIDevSettings = GetDefault<UUIDeveloperSettings>();

	if(!UIDevSettings->UIClassesMap.Contains(InWidgetTag))
	{
		PXII_LOG(ELogCategory::UI, Warning, TEXT("[%s]: Widget class %s is not assigned in Developer Settings"), *ThisClass::StaticClass()->GetName(), *InWidgetTag.ToString());
		return nullptr;
	}

	return UIDevSettings->UIClassesMap.FindRef(InWidgetTag);
}