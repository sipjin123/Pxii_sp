// Fill out your copyright notice in the Description page of Project Settings.

#include "Utility/PxiiUIBPLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/UIDeveloperSettings.h"
#include "Utility/PXIILogUtility.h"

// TSoftClassPtr<UPxiiActivatableWidget> UPxiiUIBPLibrary::GetWidgetSoftClassByTag(const UObject* WorldContextObject, UPARAM(meta = (Categories = "Pxii.UI.Widget")) const FGameplayTag InWidgetTag)
// {
// 	UPxiiUISubsystem* UISubsystem = UPxiiUISubsystem::Get(WorldContextObject);
//
// 	if (UISubsystem)
// 	{
// 		if (UISubsystem->DataMap)
// 		{
// 			if(!UISubsystem->DataMap->UIClassesMap.Contains(InWidgetTag))
// 			{
// 				PXII_LOG(ELogCategory::UI, Warning, TEXT("[%s]: Widget class %s is not assigned in %s"), *ThisClass::StaticClass()->GetName(), *InWidgetTag.ToString(), *UISubsystem->DataMap->GetName());
// 				return nullptr;
// 			}
// 		
// 			return UISubsystem->DataMap->UIClassesMap.FindRef(InWidgetTag);
// 		}
// 		else
// 		{
// 			PxiiLog::Print(ThisClass::StaticClass()->GetName(), TEXT("DATA MAP IS NOT ASSIGNED"), ELogCategory::UI, EPXIILogVerbosity::Error);
// 		}
// 	}
// 	
// 	return nullptr;
// }