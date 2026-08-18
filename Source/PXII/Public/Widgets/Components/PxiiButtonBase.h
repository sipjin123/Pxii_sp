// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "PxiiButtonBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class PXII_API UPxiiButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetButtonText(const FText& InButtonText);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetButtonToolTipText(const FText& InButtonDescriptionText);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetButtonTextAlignment(const EHorizontalAlignment& InHorizontalAlignment, const EVerticalAlignment& InVerticalAlignment);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetShowToolTip(bool InShowToolTip);
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	FText GetButtonText() const;
	
private:
	//~ Begin UCommonButtonBase Interface
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	//~ End UCommonButtonBase Interface
	
	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface

	// **** Bound Widgets **** //
	UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* TextBlock_ButtonText;
	// **** Bound Widgets **** //

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI | Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonText;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI | Button", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EHorizontalAlignment> ButtonTextHorizontalAlignment;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI | Button", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EVerticalAlignment> ButtonTextVerticalAlignment;

	// Can be use for tooltip, or use for button description
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI | Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDescriptionText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI | Button", meta = (AllowPrivateAccess = "true"))
	bool bUseUpperCaseForButtonText { false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI | Button", meta = (AllowPrivateAccess = "true"))
	bool bShowToolTip { false };
};
