// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Components/PxiiRotatorBase.h"
#include "CommonTextBlock.h"

void UPxiiRotatorBase::SetSelectedOptionByText(const FText& InText)
{
	const int32 FoundIndex = TextLabels.IndexOfByPredicate(
		[InText](const FText& TextItem)->bool
		{
			return TextItem.EqualTo(InText);
		}
	);

	if (FoundIndex != INDEX_NONE)
	{
		SetSelectedItem(FoundIndex);
	}
	else
	{
		MyText->SetText(InText);
	}
}
