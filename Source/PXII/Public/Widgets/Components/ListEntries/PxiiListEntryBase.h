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
	
protected:
	//~ Begin IUserObjectListEntry Interface
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//~ End IUserObjectListEntry Interface

	// Child class should override this function to handle the initialization. Super is expected.
	virtual void OnOwningListDataObjectSet(UPxiiListDataObjectBase* InOwningListDataObject);

	// Child class should override this function to update UI values after data object is modified. Super is no need.
	virtual void OnOwningListDataObjectModified(UPxiiListDataObjectBase* InModifiedListDataObject, EListDataModifyType InModifyType);

private:
#pragma region Bound Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonTextBlock* TextBlock_EntryDisplayName;
#pragma endregion 
};
