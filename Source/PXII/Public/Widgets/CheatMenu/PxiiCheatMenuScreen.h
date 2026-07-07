// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/PxiiWidgetBase.h"
#include "Widgets/Components/PxiiButtonBase.h"
#include "Widgets/Components/PxiiTabListWidgetBase.h"
#include "Widgets/Components/PxiiListViewBase.h"
#include "PxiiCheatMenuScreen.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiCheatMenuScreen : public UPxiiWidgetBase
{
	GENERATED_BODY()
	
protected:
	// ~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	// ~ End UUserWidget Interface

private:
	// Debug
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI | Cheat Menu", meta = (AllowPrivateAccess = "true"))
	TArray<FName> CheatMenuTabIDs;

	TArray<UPxiiButtonBase*> CheatMenuTabButtons;
	// Debug

	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

#pragma region Bound Widgets
	UPROPERTY(meta = (BindWidget))
	UPxiiTabListWidgetBase* TabListWidget_CheatTabs;

	UPROPERTY(meta = (BindWidget))
	UPxiiListViewBase* ListView_CheatList;
#pragma endregion Bound Widgets

	UFUNCTION()
	void OnTabSelected(FName TabID);
};
