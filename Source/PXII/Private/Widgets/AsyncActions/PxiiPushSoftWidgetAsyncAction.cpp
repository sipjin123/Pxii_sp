// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AsyncActions/PxiiPushSoftWidgetAsyncAction.h"
#include "Subsystem/PxiiUISubsystem.h"
#include "Utility/PXIILogUtility.h"

void UPxiiPushSoftWidgetAsyncAction::Activate()
{
	UPxiiUISubsystem* UISubsystem = UPxiiUISubsystem::Get(CachedOwningWorld.Get());
	if (UISubsystem)
	{
		UISubsystem->PushSoftWidgetToStackAsync(CachedStackTag, CachedSoftWidgetClass, [this](EAsyncPushWidgetState InState, UPxiiActivatableWidget* InPushedWidget)
		{
			switch(InState)
			{
				case EAsyncPushWidgetState::OnCreatedBeforePush:
					InPushedWidget->SetOwningPlayer(CachedOwningPlayerController.Get());
					OnWidgetCreatedBeforePush.Broadcast(InPushedWidget);
					break;
				case EAsyncPushWidgetState::AfterPush:
					OnAfterPushedWidget.Broadcast(InPushedWidget);
					if(bCachedFocusOnNewlyPushedWidget)
					{
						if(UWidget* WidgetToFocus = InPushedWidget->GetDesiredFocusTarget())
						{
							WidgetToFocus->SetFocus();
						}
					}
					SetReadyToDestroy();
					break;
				default:
					break;
			}
		});
	}
}

UPxiiPushSoftWidgetAsyncAction* UPxiiPushSoftWidgetAsyncAction::PushSoftWidget(const UObject* WorldContextObject,
														  APxiiPlayerController* OwningPlayerController,
														  TSoftClassPtr<UPxiiActivatableWidget> InSoftWidgetClass,
														  UPARAM(meta = (Categories = "UI.WidgetStack")) FGameplayTag InStackTag, bool bFocusOnNewlyPushedWidget)
{
	if(InSoftWidgetClass.IsNull())
	{
		PXII_LOG(ELogCategory::UI, Warning, TEXT("[%s]: Soft Widget Class is Null."), *FString(__FUNCTION__));
		return nullptr;
	}

	if(GEngine)
	{
		if(UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UPxiiPushSoftWidgetAsyncAction* Node = NewObject<UPxiiPushSoftWidgetAsyncAction>();

			Node->CachedOwningWorld = World;
			Node->CachedOwningPlayerController = OwningPlayerController;
			Node->CachedSoftWidgetClass = InSoftWidgetClass;
			Node->CachedStackTag = InStackTag;
			Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;

			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}

	return nullptr;
}
