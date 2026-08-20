// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PxiiListDataObjectValue.h"
#include "PxiiListDataObjectString.generated.h"

/**
 *
 */
UCLASS()
class PXII_API UPxiiListDataObjectString : public UPxiiListDataObjectValue
{
	GENERATED_BODY()

public:
	void AddDynamicOptions(const FString& InStringValue, const FText& InDisplayText);
	void NextOption();
	void PreviousOption();
	void OnValueChangeByRotator(const FText& InSelectedText);
	
	FORCEINLINE const TArray<FText>& GetAvailableDisplayTexts() const { return AvailableDisplayTexts; }
	FORCEINLINE FText GetCurrentDisplayText() const { return CurrentDisplayText; }
	
	//~ Begin UPxiiListDataObjectBase Interface
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	//~ End UPxiiListDataObjectBase Interface
	
protected:
	//~ Begin UPxiiListDataObjectBase Interface
	virtual void OnDataObjectInitialized() override;
	//~ End UPxiiListDataObjectBase Interface
	
	bool TrySetDisplayText(const FString& InStringValue);
	
	FString CurrentStringValue;
	FText CurrentDisplayText;
	TArray<FString> AvailableStringValues;
	TArray<FText> AvailableDisplayTexts;
};