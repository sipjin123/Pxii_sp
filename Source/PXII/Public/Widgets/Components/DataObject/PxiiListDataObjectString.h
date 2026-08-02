// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectBase.h"
#include "PxiiListDataObjectString.generated.h"

/**
 *
 */
UCLASS()
class PXII_API UPxiiListDataObjectString : public UPxiiListDataObjectBase
{
	GENERATED_BODY()

public:
	void AddDynamicOptions(const FString& InStringValue, const FText& InDisplayText);
	void NextOption();
	void PreviousOption();
	
	FORCEINLINE const TArray<FText>& GetAvailableDisplayTexts() const { return AvailableDisplayTexts; }
	FORCEINLINE FText GetCurrentDisplayText() const { return CurrentDisplayText; }
	
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