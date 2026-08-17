// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUI/PxiiCommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "CommonTextBlock.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectBase.h"
#include "PxiiListEntryBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class PXII_API UPxiiListEntryBase : public UPxiiCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "List Entry | Style")
	TSubclassOf<UCommonTextStyle> DefaultTextStyle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "List Entry | Style")
	TSubclassOf<UCommonTextStyle> HighlightTextStyle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "List Entry | Style")
	TSubclassOf<UCommonTextStyle> DisableTextStyle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "List Entry | Style")
	TSubclassOf<UCommonButtonStyle> DefaultButtonStyle;
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On List Entry Widget Hovered"))
	void BP_OnListEntryWidgetHovered(bool bWasHovered, bool bIsEntryWidgetStillSelected);
	
	void NativeOnListEntryWidgetHovered(bool bWasHovered);
	
protected:
	// Child WBP should override this to get gamepad function properly
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Gamepad Get Widget To Focus"))
	UWidget* BP_GamepadGetWidgetToFocus();
	
	//~ Begin IUserObjectListEntry Interface
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnEntryReleased() override;
	//~ End IUserObjectListEntry Interface
	
	//~ Begin UUserWidget Interface
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	//~ End UUserWidget Interface

	// Child class should override this function to handle the initialization. Super is expected.
	virtual void OnOwningListDataObjectSet(UPxiiListDataObjectBase* InOwningListDataObject);

	// Child class should override this function to update UI values after data object is modified. Super is no need.
	virtual void OnOwningListDataObjectModified(UPxiiListDataObjectBase* InModifiedListDataObject, EListDataModifyType InModifyType);

	void SelectThisEntryWidget();
	
private:
#pragma region Bound Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonTextBlock* TextBlock_EntryDisplayName;
#pragma endregion 
};
