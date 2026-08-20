// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/ListEntries/PxiiListEntryString.h"

#include "CommonInputSubsystem.h"
#include "Utility/PXIILogUtility.h"
#include "Widgets/Components/PxiiListViewBase.h"

void UPxiiListEntryString::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	Button_Previous->OnClicked().AddUObject(this, &ThisClass::OnPreviousButtonClicked);
	Button_Next->OnClicked().AddUObject(this, &ThisClass::OnNextButtonClicked);
	
	Rotator_Options->OnClicked().AddLambda([this]()
		{
			SelectThisEntryWidget();
		}
	);
	
	Rotator_Options->OnRotatedEvent.AddUObject(this, &UPxiiListEntryString::OnRotatorValueChanged);
}

void UPxiiListEntryString::OnOwningListDataObjectSet(UPxiiListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);
	
	CachedOwningListDataObjectString = Cast<UPxiiListDataObjectString>(InOwningListDataObject);
	
	if (!IsValid(CachedOwningListDataObjectString))
	{
		PXII_LOG(ELogCategory::UI, Warning, TEXT("[% s]: Failed to cast to list data object string"), *ThisClass::StaticClass()->GetName());
	}
	
	Rotator_Options->PopulateTextLabels(CachedOwningListDataObjectString->GetAvailableDisplayTexts());
	Rotator_Options->SetSelectedOptionByText(CachedOwningListDataObjectString->GetCurrentDisplayText());
	
	{// for disabling reso change when fullscreen
		if (CachedOwningListDataObjectString->GetDataDisplayName().EqualTo(FText::FromString("Display Resolution")))
		{
			UPxiiListViewBase* OwningListView = Cast<UPxiiListViewBase>(GetOwningListView());
		
			if (!OwningListView)
			{
				return;
			}
		
			OwningListView->SetCachedDisplayResolutionListEntry(this);
		}
	}
}

void UPxiiListEntryString::OnOwningListDataObjectModified(UPxiiListDataObjectBase* InModifiedListDataObject,
	EListDataModifyType InModifyType)
{
	if (CachedOwningListDataObjectString)
	{
		Rotator_Options->SetSelectedOptionByText(CachedOwningListDataObjectString->GetCurrentDisplayText());
	}
	
	{// for disabling reso change when fullscreen
		if (CachedOwningListDataObjectString->GetDataDisplayName().EqualTo(FText::FromString("Display Mode")))
		{
			UPxiiListViewBase* OwningListView = Cast<UPxiiListViewBase>(GetOwningListView());
		
			if (!OwningListView)
			{
				return;
			}
		
			if (OwningListView->GetCachedDisplayResolutionListEntry())
			{
				if (CachedOwningListDataObjectString->GetCurrentDisplayText().EqualTo(FText::FromString("Fullscreen")) ||
				   CachedOwningListDataObjectString->GetCurrentDisplayText().EqualTo(FText::FromString("WindowedFullscreen")))
				{
					OwningListView->GetCachedDisplayResolutionListEntry()->OnFullscreen.Broadcast(false);
					OwningListView->GetCachedDisplayResolutionListEntry()->GetCachedOwningListDataObjectString()->TryResetBackToDefaultValue();
				}
				else
				{
					OwningListView->GetCachedDisplayResolutionListEntry()->OnFullscreen.Broadcast(true);
				}
			}
			else
			{
				PXII_LOG(ELogCategory::UI, Warning, TEXT("[% s]: Invalid cached entry"), *ThisClass::StaticClass()->GetName());
			}
		}
	}
}

void UPxiiListEntryString::OnPreviousButtonClicked()
{
	if (CachedOwningListDataObjectString)
	{
		CachedOwningListDataObjectString->PreviousOption();
	}
	
	SelectThisEntryWidget();
}

void UPxiiListEntryString::OnNextButtonClicked()
{
	if (CachedOwningListDataObjectString)
	{
		CachedOwningListDataObjectString->NextOption();
	}
	
	SelectThisEntryWidget();
}

void UPxiiListEntryString::OnRotatorValueChanged(int32 Value, bool bUserInitiated)
{
	if (!CachedOwningListDataObjectString)
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
		CachedOwningListDataObjectString->OnValueChangeByRotator(Rotator_Options->GetSelectedText());
	}
}
