// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/Components/DataObject/PxiiListDataObjectStringImage.h"

#include "Utility/PXIILogUtility.h"

void UPxiiListDataObjectStringImage::AddDynamicOptions(const FString& InStringValue, const FText& InDisplayText, TSoftObjectPtr<UTexture2D> InTexture)
{
	if (InTexture.IsNull())
	{
		return;
	}
	AvailableStringValues.Add(InStringValue);
	AvailableDisplayTexts.Add(InDisplayText);
	AvailableTextures.Add(InTexture);
	
	if (CurrentTexture.IsNull())
	{
		CurrentTexture = InTexture;
	}
}

void UPxiiListDataObjectStringImage::NextOption()
{
	if (AvailableStringValues.IsEmpty() || AvailableDisplayTexts.IsEmpty() || AvailableTextures.IsEmpty())
	{
		return;
	}
	
	const int32 CurrentStringIndex = AvailableStringValues.IndexOfByKey(CurrentStringValue);
	const int32 NextStringIndex = CurrentStringIndex + 1;
	
	if (AvailableStringValues.IsValidIndex(NextStringIndex))
	{
		CurrentStringValue = AvailableStringValues[NextStringIndex];
	}
	else
	{
		CurrentStringValue = AvailableStringValues[0];
	}
	
	if (AvailableTextures.IsValidIndex(NextStringIndex))
	{
		CurrentTexture = AvailableTextures[NextStringIndex];
	}
	else
	{
		CurrentTexture = AvailableTextures[0];
	}
	
	TrySetDisplayText(CurrentStringValue);
	
	//Set value to settings
	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueToSaveFromString(GetDataID(), CurrentStringValue);
		
		PxiiLog::PrintOnScreen(ThisClass::StaticClass()->GetName(), FString::Printf(TEXT("Dynamic setter -> latest value: %s"), *DataDynamicGetter->GetSavedValueAsString(GetDataID())), FColor::Green);
		
		NotifyListDataModified(this);
	}
}

void UPxiiListDataObjectStringImage::PreviousOption()
{
	if (AvailableStringValues.IsEmpty() || AvailableDisplayTexts.IsEmpty() || AvailableTextures.IsEmpty())
	{
		return;
	}
	
	const int32 CurrentStringIndex = AvailableStringValues.IndexOfByKey(CurrentStringValue);
	const int32 PreviousStringIndex = CurrentStringIndex - 1;
	
	if (AvailableStringValues.IsValidIndex(PreviousStringIndex))
	{
		CurrentStringValue = AvailableStringValues[PreviousStringIndex];
	}
	else
	{
		CurrentStringValue = AvailableStringValues.Last();
	}
	
	if (AvailableTextures.IsValidIndex(PreviousStringIndex))
	{
		CurrentTexture = AvailableTextures[PreviousStringIndex];
	}
	else
	{
		CurrentTexture = AvailableTextures[0];
	}
	
	TrySetDisplayText(CurrentStringValue);
	
	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueToSaveFromString(GetDataID(), CurrentStringValue);
		
		PxiiLog::PrintOnScreen(ThisClass::StaticClass()->GetName(), FString::Printf(TEXT("Dynamic setter -> latest value: %s"), *DataDynamicGetter->GetSavedValueAsString(GetDataID())), FColor::Green);
				
		NotifyListDataModified(this);
	}
}

void UPxiiListDataObjectStringImage::OnValueChangeByRotator(const FText& InSelectedText)
{
	const int32 FoundIndex = AvailableDisplayTexts.IndexOfByPredicate(
		[InSelectedText](const FText& AvailableText)->bool
		{
			return AvailableText.EqualTo(InSelectedText);
		}
	);
	
	if (FoundIndex != INDEX_NONE && AvailableStringValues.IsValidIndex(FoundIndex))
	{
		CurrentDisplayText = InSelectedText;
		CurrentStringValue = AvailableStringValues[FoundIndex];
		
		// Save data
		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueToSaveFromString(GetDataID(), CurrentStringValue);
			
			NotifyListDataModified(this);
		}
	}
}

void UPxiiListDataObjectStringImage::OnDataObjectInitialized()
{
	if (!AvailableStringValues.IsEmpty())
	{
		CurrentStringValue = AvailableStringValues[0];
	}
	
	if (HasDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();
	}
	
	//Read the saved value to set as current value
	if (DataDynamicGetter)
	{
		if (!DataDynamicGetter->GetSavedValueAsString(GetDataID()).IsEmpty())
		{
			CurrentStringValue = DataDynamicGetter->GetSavedValueAsString(GetDataID());
		}
	}
	
	if (!TrySetDisplayText(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString("Invalid Display Text");
	}
}

bool UPxiiListDataObjectStringImage::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && CurrentStringValue != GetDefaultValueAsString();
}

bool UPxiiListDataObjectStringImage::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		CurrentStringValue = GetDefaultValueAsString();
		TrySetDisplayText(CurrentStringValue);
		
		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueToSaveFromString(GetDataID(), CurrentStringValue);
			
			NotifyListDataModified(this, EListDataModifyType::ResetToDefault);
			
			
			// Success reset
			return true;
		}
	}
	
	return false;
}

bool UPxiiListDataObjectStringImage::TrySetDisplayText(const FString& InStringValue)
{
	const int Index = AvailableStringValues.IndexOfByKey(InStringValue);
	
	if (AvailableDisplayTexts.IsValidIndex(Index))
	{
		CurrentDisplayText = AvailableDisplayTexts[Index];
		
		return true;
	}
	
	return false;
}