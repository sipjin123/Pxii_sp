// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PxiiListEntryBase.h"
#include "Widgets/Components/PxiiButtonBase.h"
#include "Widgets/Components/PxiiRotatorBase.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectStringImage.h"
#include "PxiiListEntryStringImage.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class PXII_API UPxiiListEntryStringImage : public UPxiiListEntryBase
{
	GENERATED_BODY()
	
protected:
	// ~ Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	// ~ End UUserWidget Interface
	
	//~ Begin UPxiiListEntryBase Interface
	virtual void OnOwningListDataObjectSet(UPxiiListDataObjectBase* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UPxiiListDataObjectBase* InModifiedListDataObject, EListDataModifyType InModifyType) override;
	//~ End UPxiiListEntryBase Interface
	
private:
	void OnPreviousButtonClicked();
	void OnNextButtonClicked();
	
	void OnRotatorValueChanged(int32 Value, bool bUserInitiated);
	
#pragma region BoundWidgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UPxiiButtonBase* Button_Previous;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UPxiiButtonBase* Button_Next;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UPxiiRotatorBase* Rotator_Options;
#pragma endregion
	
	UPROPERTY(Transient)
	UPxiiListDataObjectStringImage* CachedOwningListDataObjectStringImage;
};
