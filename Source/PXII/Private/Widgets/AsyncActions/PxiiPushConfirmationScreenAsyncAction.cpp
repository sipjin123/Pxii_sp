// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AsyncActions/PxiiPushConfirmationScreenAsyncAction.h"
#include "Engine/Engine.h"

void UPxiiPushConfirmationScreenAsyncAction::Activate()
{
	UPxiiUISubsystem* UISubsystem = UPxiiUISubsystem::Get(CachedOwningWorld.Get());
	if (UISubsystem)
	{
		UISubsystem->PushConfirmationScreenToModalStackAsync(
			CachedScreenTitle, 
			CachedScreenContent, 
			CachedButtonsToCreate,
			[this](EConfirmationScreenButtonAction ClickedButtonAction)
			{
				OnButtonClicked.Broadcast(ClickedButtonAction);
				
				SetReadyToDestroy();
			}
		);
	}
}

UPxiiPushConfirmationScreenAsyncAction* UPxiiPushConfirmationScreenAsyncAction::PushConfirmScreen(
	const UObject* WorldContextObject, const FText InScreenTitle, const FText InScreenContent,
	TArray<FConfirmationScreenButtonInfo> ButtonsToCreate)
{
	if(GEngine)
	{
		if(UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UPxiiPushConfirmationScreenAsyncAction* Node = NewObject<UPxiiPushConfirmationScreenAsyncAction>();

			Node->CachedOwningWorld = World;
			Node->CachedScreenTitle = InScreenTitle;
			Node->CachedScreenContent = InScreenContent;
			Node->CachedButtonsToCreate = ButtonsToCreate;
			
			Node->RegisterWithGameInstance(World);

			return Node;
		}
	}
	
	return nullptr;
}
