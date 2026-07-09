// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/PxiiButtonBase.h"
#include "CommonTextBlock.h"
#include "Components/OverlaySlot.h"

void UPxiiButtonBase::SetButtonText(const FText& InButtonText)
{
	if (!TextBlock_ButtonText || InButtonText.IsEmpty())
	{
		return;
	}

	TextBlock_ButtonText->SetText(bUseUpperCaseForButtonText ? InButtonText.ToUpper() : InButtonText);

	SetButtonTextAlignment(ButtonTextHorizontalAlignment, ButtonTextVerticalAlignment);
}

void UPxiiButtonBase::SetButtonToolTipText(const FText& InButtonDescriptionText)
{
	if (bShowToolTip)
	{
		SetToolTipText(InButtonDescriptionText);
	}
}

void UPxiiButtonBase::SetButtonTextAlignment(const EHorizontalAlignment& InHorizontalAlignment, const EVerticalAlignment& InVerticalAlignment)
{
	if (UOverlaySlot* OverlaySlot = Cast<UOverlaySlot>(TextBlock_ButtonText->Slot))
	{
		OverlaySlot->SetHorizontalAlignment(InHorizontalAlignment);
		OverlaySlot->SetVerticalAlignment(InVerticalAlignment);
	}
}

void UPxiiButtonBase::SetShowToolTip(bool InShowToolTip)
{
	bShowToolTip = InShowToolTip;
}

void UPxiiButtonBase::NativeOnCurrentTextStyleChanged()
{}

void UPxiiButtonBase::NativeOnHovered()
{}

void UPxiiButtonBase::NativeOnUnhovered()
{}

void UPxiiButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonText);
}
