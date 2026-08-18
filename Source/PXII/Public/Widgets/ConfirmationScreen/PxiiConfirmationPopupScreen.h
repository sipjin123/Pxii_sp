// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "CommonUI/PxiiActivatableWidget.h"
#include "Components/DynamicEntryBox.h"
#include "Subsystem/PxiiUISubsystem.h"
#include "PxiiConfirmationPopupScreen.generated.h"

UCLASS()
class PXII_API UPxiiConfirmationScreenInfo : public UObject
{
	GENERATED_BODY()

public:
	static UPxiiConfirmationScreenInfo* CreateScreen(const FText& InScreenTitle, const FText& InScreenContent, 
		TArray<FConfirmationScreenButtonInfo> ButtonsToCreate);
	
	UPROPERTY(Transient)
	FText ScreenTitle;
	
	UPROPERTY(Transient)
	FText ScreenContent;
	
	UPROPERTY(Transient)
	TArray<FConfirmationScreenButtonInfo> AvailableScreenButtons;
};

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class PXII_API UPxiiConfirmationPopupScreen : public UPxiiActivatableWidget
{
	GENERATED_BODY()
	
public:
	// Get called outside when this widget is constructed and before pushed to stack
	void InitConfirmationScreen(UPxiiConfirmationScreenInfo* InScreenInfo, TFunction<void(EConfirmationScreenButtonAction)> ClickedButtonCallback);
	
private:
#pragma region Bound Widgets
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* TextBlock_Title;
	
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* TextBlock_Content;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UDynamicEntryBox* EntryBox_Buttons;
#pragma endregion 
};
