// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PxiiOptionsDetailsView.h"
#include "CommonUI/PxiiActivatableWidget.h"
#include "Subsystem/PxiiUISubsystem.h"
#include "Widgets/Components/PxiiListViewBase.h"
#include "Widgets/Components/PxiiTabListWidgetBase.h"
#include "Widgets/Components/DataRegistries/PxiiOptionsDataRegistry.h"
#include "PxiiOptionsMenuScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResetTriggered, FText, SelectedTabID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartReset, EConfirmationScreenButtonAction, ButtonAction);

/**
 * 
 */
UCLASS()
class PXII_API UPxiiOptionsMenuScreen : public UPxiiActivatableWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnResetTriggered OnResetTriggered;
	
	UPROPERTY(BlueprintCallable)
	FOnStartReset OnStartReset;
	
protected:
	// ~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	// ~ End UUserWidget Interface

	//~ Begin UCommonActivatableWidget Interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface

private:
	UPxiiOptionsDataRegistry* GetOrCreateDataRegistry();

	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();
	
	FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;

#pragma region Bound Widgets
	UPROPERTY(meta = (BindWidget))
	UPxiiTabListWidgetBase* TabListWidget_OptionsTabs;

	UPROPERTY(meta = (BindWidget))
	UPxiiListViewBase* ListView_OptionsList;
	
	UPROPERTY(meta = (BindWidget))
	UPxiiOptionsDetailsView* OptionsDetailsView;
#pragma endregion Bound Widgets

	//~ Begin UWidget Interface
#if WITH_EDITOR	
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface
	
	//Handle creation of data in this screen. Direct access to this variable is forbidden.
	UPROPERTY(Transient)
	UPxiiOptionsDataRegistry* CreatedOwningDataRegistry;

	UPROPERTY(EditDefaultsOnly, Category = "UI | Input", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI | Input", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle OptionsBackAction;

	UPROPERTY(EditDefaultsOnly, Category = "UI | Data")
	UPxiiOptionsData* OptionsData;
	
	FUIActionBindingHandle ResetActionHandle;
	FUIActionBindingHandle BackActionHandle;
	
	bool bIsResettingData = false;

	UFUNCTION()
	void OnTabSelected(FName TabID);
	
	UFUNCTION()
	void OnListViewItemHovered(UObject* InHoveredItem , bool bWasHovered);
	
	UFUNCTION()
	void OnListViewItemSelected(UObject* InSelectedItem);
	
	UFUNCTION()
	void OnListViewDataObjectModified(UPxiiListDataObjectBase* InModifiedDataObject, EListDataModifyType ModifiedReason);
	
	UFUNCTION()
	void ResetData(EConfirmationScreenButtonAction ButtonAction);
	
	// Resettable data
	UPROPERTY(Transient)
	TArray<UPxiiListDataObjectBase*> ResettableDataArray;
};
