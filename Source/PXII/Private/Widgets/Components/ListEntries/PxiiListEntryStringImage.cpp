// Fill out your copyright notice in the Description page of Project Settings.\

#include "Widgets/Components/ListEntries/PxiiListEntryStringImage.h"
#include "CommonInputSubsystem.h"
#include "Utility/PXIILogUtility.h"

void UPxiiListEntryStringImage::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Button_Previous->OnClicked().AddUObject(this, &ThisClass::OnPreviousButtonClicked);
	Button_Next->OnClicked().AddUObject(this, &ThisClass::OnNextButtonClicked);
	
	Rotator_Options->OnClicked().AddLambda([this]()
		{
			SelectThisEntryWidget();
		}
	);
	
	Rotator_Options->OnRotatedEvent.AddUObject(this, &UPxiiListEntryStringImage::OnRotatorValueChanged);
}

void UPxiiListEntryStringImage::OnOwningListDataObjectSet(UPxiiListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
	
	CachedOwningListDataObjectStringImage = Cast<UPxiiListDataObjectStringImage>(InOwningListDataObject);
	
	if (!IsValid(CachedOwningListDataObjectStringImage))
	{
		PXII_LOG(ELogCategory::UI, Warning, TEXT("[% s]: Failed to cast to list data object string image"), *ThisClass::StaticClass()->GetName());
	}
	
	Rotator_Options->PopulateTextLabels(CachedOwningListDataObjectStringImage->GetAvailableDisplayTexts());
	Rotator_Options->SetSelectedOptionByText(CachedOwningListDataObjectStringImage->GetCurrentDisplayText());
}

void UPxiiListEntryStringImage::OnOwningListDataObjectModified(UPxiiListDataObjectBase* InModifiedListDataObject,
	EListDataModifyType InModifyType)
{
	if (CachedOwningListDataObjectStringImage)
	{
		Rotator_Options->SetSelectedOptionByText(CachedOwningListDataObjectStringImage->GetCurrentDisplayText());
	}
}

void UPxiiListEntryStringImage::OnPreviousButtonClicked()
{
	if (CachedOwningListDataObjectStringImage)
	{
		CachedOwningListDataObjectStringImage->PreviousOption();
	}
	
	SelectThisEntryWidget();
}

void UPxiiListEntryStringImage::OnNextButtonClicked()
{
	if (CachedOwningListDataObjectStringImage)
	{
		CachedOwningListDataObjectStringImage->NextOption();
	}
	
	SelectThisEntryWidget();
}

void UPxiiListEntryStringImage::OnRotatorValueChanged(int32 Value, bool bUserInitiated)
{
	if (!CachedOwningListDataObjectStringImage)
	{
		return;
	}
	
	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
	if (!CommonInputSubsystem || !bUserInitiated)
	{
		return;
	}
	
	if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		CachedOwningListDataObjectStringImage->OnValueChangeByRotator(Rotator_Options->GetSelectedText());
	}
}