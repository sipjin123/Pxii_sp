// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ConfirmationScreen/PxiiConfirmationPopupScreen.h"
#include "ICommonInputModule.h"
#include "Utility/PXIILogUtility.h"

UPxiiConfirmationScreenInfo* UPxiiConfirmationScreenInfo::CreateScreen(const FText& InScreenTitle,
                                                                                  const FText& InScreenContent, 
                                                                                  TArray<FConfirmationScreenButtonInfo> ButtonsToCreate)
{
	if (ButtonsToCreate.IsEmpty())
	{
		PXII_LOG(ELogCategory::UI, Warning, TEXT("[% s]: Failed to create confirmation screen"), *ThisClass::StaticClass()->GetName());
		return nullptr;
	}
	
	UPxiiConfirmationScreenInfo* InfoObject = NewObject<UPxiiConfirmationScreenInfo>();
	InfoObject->ScreenTitle = InScreenTitle;
	InfoObject->ScreenContent = InScreenContent;
	
	for (FConfirmationScreenButtonInfo Button : ButtonsToCreate)
	{
		FConfirmationScreenButtonInfo ButtonInfo;
		ButtonInfo.ButtonAction = Button.ButtonAction;
		ButtonInfo.ButtonText = Button.ButtonText;
		
		InfoObject->AvailableScreenButtons.Add(ButtonInfo);
	}
	
	return InfoObject;
}

void UPxiiConfirmationPopupScreen::InitConfirmationScreen(UPxiiConfirmationScreenInfo* InScreenInfo,
	TFunction<void(EConfirmationScreenButtonAction)> ClickedButtonCallback)
{
	if (!InScreenInfo || !TextBlock_Title || !TextBlock_Content || !EntryBox_Buttons)
	{
		PXII_LOG(ELogCategory::UI, Warning, TEXT("[% s]: Title or content or entry box or screen info is invalid"), *ThisClass::StaticClass()->GetName());
		return;
	}
	
	TextBlock_Title->SetText(InScreenInfo->ScreenTitle);
	TextBlock_Content->SetText(InScreenInfo->ScreenContent);
	
	// If entry box has old created buttons
	if (EntryBox_Buttons->GetNumEntries() > 0)
	{
		// Clear the old buttons
		EntryBox_Buttons->Reset<UPxiiButtonBase>(
			[](UPxiiButtonBase& ExistingButton)
			{
				ExistingButton.OnClicked().Clear();
			}	
		);
	}
	
	if (InScreenInfo->AvailableScreenButtons.IsEmpty())
	{
		PXII_LOG(ELogCategory::UI, Warning, TEXT("[% s]: No buttons to be created"), *ThisClass::StaticClass()->GetName());
		return;
	}
	
	for (const FConfirmationScreenButtonInfo& AvailableButton : InScreenInfo->AvailableScreenButtons)
	{
		FDataTableRowHandle InputActionRowHandle;
		
		switch (AvailableButton.ButtonAction)
		{
			case EConfirmationScreenButtonAction::Cancelled:
				InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
				break;
			case EConfirmationScreenButtonAction::Closed:
				InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
				break;
			default:
				break;
		}
		
		UPxiiButtonBase* ButtonCreated = EntryBox_Buttons->CreateEntry<UPxiiButtonBase>();
		ButtonCreated->SetButtonText(AvailableButton.ButtonText);
		
		ButtonCreated->SetTriggeringInputAction(InputActionRowHandle);
		ButtonCreated->OnClicked().AddLambda(
			[ClickedButtonCallback, AvailableButton, this]()
			{
				ClickedButtonCallback(AvailableButton.ButtonAction);
				
				DeactivateWidget();
			}	
		);
	}
	
	if (EntryBox_Buttons->GetNumEntries() > 0)
	{
		// Set focus on last button, which typically no/ concel
		EntryBox_Buttons->GetAllEntries().Last()->SetFocus();
		
		// Prevent out of bounds focus
		EntryBox_Buttons->GetAllEntries().Last()->SetNavigationRuleExplicit(EUINavigation::Up, EntryBox_Buttons->GetAllEntries().Last());
		EntryBox_Buttons->GetAllEntries().Last()->SetNavigationRuleExplicit(EUINavigation::Down, EntryBox_Buttons->GetAllEntries().Last());
		EntryBox_Buttons->GetAllEntries().Last()->SetNavigationRuleExplicit(EUINavigation::Right, EntryBox_Buttons->GetAllEntries().Last());
		EntryBox_Buttons->GetAllEntries()[0]->SetNavigationRuleExplicit(EUINavigation::Left, EntryBox_Buttons->GetAllEntries()[0]);
	}
}
