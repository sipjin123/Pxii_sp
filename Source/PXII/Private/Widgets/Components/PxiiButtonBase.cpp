// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/PxiiButtonBase.h"
#include "CommonTextBlock.h"
#include "Components/OverlaySlot.h"
#include "Subsystem/PxiiUISubsystem.h"

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

FText UPxiiButtonBase::GetButtonText() const
{
	if (TextBlock_ButtonText)
	{
		return TextBlock_ButtonText->GetText();
	}
	
	return FText();
}

void UPxiiButtonBase::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();

	if (TextBlock_ButtonText && GetCurrentTextStyleClass())
	{
		TextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
	}
}

void UPxiiButtonBase::NativeOnHovered()
{
	Super::NativeOnHovered();
	
	if (!ButtonDescriptionText.IsEmpty())
	{
		UPxiiUISubsystem::Get(this)->OnButtonTextDescriptionUpdated.Broadcast(this, ButtonDescriptionText);
	}
}

void UPxiiButtonBase::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();
	
	if (!ButtonDescriptionText.IsEmpty())
	{
		UPxiiUISubsystem::Get(this)->OnButtonTextDescriptionUpdated.Broadcast(this, FText::GetEmpty());
	}
}

void UPxiiButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonText);
}
