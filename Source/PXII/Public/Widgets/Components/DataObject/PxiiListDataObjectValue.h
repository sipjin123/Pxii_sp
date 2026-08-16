// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PxiiListDataObjectBase.h"
#include "Utility/PxiiDataInteractionUtility.h"
#include "PxiiListDataObjectValue.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiListDataObjectValue : public UPxiiListDataObjectBase
{
	GENERATED_BODY()
	
public:
	void SetDataDynamicGetter(const TSharedPtr<FPxiiDataInteractionUtility>& InDynamicGetter);
	void SetDataDynamicSetter(const TSharedPtr<FPxiiDataInteractionUtility>& InDynamicSetter);
	
	FORCEINLINE void SetDefaultValueFromString(const FString& InDefaultValue) { DefaultStringValue = InDefaultValue; }
	
	//~ Begin UListDataObject_Base Interface
	virtual bool HasDefaultValue() const override { return DefaultStringValue.IsSet();}
	//~ End UListDataObject_Base Interface
	
protected:
	FORCEINLINE FString GetDefaultValueFromString() const { return DefaultStringValue.GetValue(); }
	
	TSharedPtr<FPxiiDataInteractionUtility> DataDynamicGetter;
	TSharedPtr<FPxiiDataInteractionUtility> DataDynamicSetter;
	
private:
	TOptional<FString> DefaultStringValue;
};
