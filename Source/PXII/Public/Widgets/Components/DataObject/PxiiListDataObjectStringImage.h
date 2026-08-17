// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PxiiListDataObjectValue.h"
#include "PxiiListDataObjectStringImage.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiListDataObjectStringImage : public UPxiiListDataObjectValue
{
	GENERATED_BODY()
	
public:
	void AddDynamicOptions(const FString& InStringValue, const FText& InDisplayText, TSoftObjectPtr<UTexture2D> InTexture);
	void NextOption();
	void PreviousOption();
	void OnValueChangeByRotator(const FText& InSelectedText);
	
	FORCEINLINE const TArray<FText>& GetAvailableDisplayTexts() const { return AvailableDisplayTexts; }
	FORCEINLINE FText GetCurrentDisplayText() const { return CurrentDisplayText; }
	FORCEINLINE TSoftObjectPtr<UTexture2D> GetCurrentTexture() const { return CurrentTexture; }
	
protected:
	//~ Begin UPxiiListDataObjectBase Interface
	virtual void OnDataObjectInitialized() override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	//~ End UPxiiListDataObjectBase Interface
	
	bool TrySetDisplayText(const FString& InStringValue);
	
	FString CurrentStringValue;
	FText CurrentDisplayText;
	TSoftObjectPtr<UTexture2D> CurrentTexture;
	TArray<FString> AvailableStringValues;
	TArray<FText> AvailableDisplayTexts;
	TArray<TSoftObjectPtr<UTexture2D>> AvailableTextures;
};
