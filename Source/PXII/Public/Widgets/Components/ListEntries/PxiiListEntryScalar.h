// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnalogSlider.h"
#include "CommonNumericTextBlock.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectScalar.h"
#include "Widgets/Components/ListEntries/PxiiListEntryBase.h"
#include "PxiiListEntryScalar.generated.h"

/**
 *
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class PXII_API UPxiiListEntryScalar : public UPxiiListEntryBase
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
	UFUNCTION()
	void OnSliderValueChange(float Value);
	
#pragma region BoundWidgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCommonNumericTextBlock* NumericTextBlock_Value;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UAnalogSlider* AnalogSlider_Slider;
#pragma endregion
	
	UPROPERTY(Transient)
	UPxiiListDataObjectScalar* CachedOwningListDataObjectScalar;
	
	bool bIsInitializing = false;
};