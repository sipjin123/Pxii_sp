// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OptionsMenu/PxiiOptionsDetailsView.h"

void UPxiiOptionsDetailsView::UpdateDetailsInfo(UPxiiListDataObjectBase* InDataObject,
	const FString& InEntryWidgetClassName)
{
	if (!InDataObject)
	{
		return;
	}
	
	TextBlock_Title->SetText(InDataObject->GetDataDisplayName());
	
	if (!InDataObject->GetSoftDescriptionImage().IsNull())
	{
		Image_DescriptionImage->SetBrushFromLazyTexture(InDataObject->GetSoftDescriptionImage());
		Image_DescriptionImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	
	RichTextBlock_Description->SetText(InDataObject->GetDescriptionRichText());
	
	// TODO
	//RichTextBlock_DynamicDetails->SetText();
	
	RichTextBlock_DisabledReason->SetText(InDataObject->GetDisabledRichText());
}

void UPxiiOptionsDetailsView::ClearDetailsInfo()
{
	TextBlock_Title->SetText(FText::GetEmpty());
	Image_DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	RichTextBlock_Description->SetText(FText::GetEmpty());
	RichTextBlock_DynamicDetails->SetText(FText::GetEmpty());
	RichTextBlock_DisabledReason->SetText(FText::GetEmpty());
}

void UPxiiOptionsDetailsView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ClearDetailsInfo();
}
