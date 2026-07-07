// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CheatMenu/PxiiCheatMenuScreen.h"

void UPxiiCheatMenuScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// For reset action


	//TabListWidget_CheatTabs->OnTabSelected.AddDynamic(this, &UPxiiCheatMenuScreen::OnTabSelected);
}

void UPxiiCheatMenuScreen::NativeConstruct()
{
	Super::NativeConstruct();

	if (!CheatMenuTabIDs.IsEmpty())
	{
		for (int32 i = 0; i < CheatMenuTabIDs.Num(); ++i)
		{
			CheatMenuTabButtons.AddUnique(TabListWidget_CheatTabs->RequestRegisterTab(CheatMenuTabIDs[i]));
		}
	}

	APlayerController* PC = GetOwningPlayer();
	FInputModeGameAndUI InputMode;
	PC->SetInputMode(InputMode);
	PC->SetShowMouseCursor(true);
}

void UPxiiCheatMenuScreen::NativeDestruct()
{
	APlayerController* PC = GetOwningPlayer();
	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->SetShowMouseCursor(false);

	Super::NativeDestruct();
}

void UPxiiCheatMenuScreen::OnResetBoundActionTriggered()
{
}

void UPxiiCheatMenuScreen::OnBackBoundActionTriggered()
{
}

void UPxiiCheatMenuScreen::OnTabSelected(FName TabID)
{
}
