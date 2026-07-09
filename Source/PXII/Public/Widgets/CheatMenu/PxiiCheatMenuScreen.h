// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUI/PxiiActivatableWidget.h"
#include "Widgets/Components/PxiiButtonBase.h"
#include "Widgets/Components/PxiiTabListWidgetBase.h"
#include "Widgets/Components/PxiiListViewBase.h"
#include "PxiiCheatMenuScreen.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiCheatMenuScreen : public UPxiiActivatableWidget
{
	GENERATED_BODY()
	
protected:
	// ~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	// ~ End UUserWidget Interface

	//~ Begin UCommonActivatableWidget Interface
	virtual void NativeOnActivated() override;
	//~ End UCommonActivatableWidget Interface

private:
	//UDataRegistry_Options* GetOrCreateDataRegistry();

	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	UFUNCTION()
	void OnTabSelected(FName TabID);

#pragma region Bound Widgets
	UPROPERTY(meta = (BindWidget))
	UPxiiTabListWidgetBase* TabListWidget_CheatTabs;

	UPROPERTY(meta = (BindWidget))
	UPxiiListViewBase* ListView_CheatList;
#pragma endregion Bound Widgets

	//Handle creation of data in the options screen. Direct access to this variable is forbidden.
	//UPROPERTY(Transient)
	//UDataRegistry_Options* CreatedOwningDataRegistry;

	UPROPERTY(EditDefaultsOnly, Category = "Option Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;
};
