// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "UObject/Object.h"
#include "PxiiListDataObjectBase.generated.h"

#define LIST_DATA_ACCESSOR(DataType, PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const { return PropertyName; }; \
	FORCEINLINE void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName; };

UENUM(BlueprintType)
enum class EListDataModifyType : uint8
{
	DirectlyModified,
	DependencyModified,
	ResetToDefault
};

/**
 * 
 */
UCLASS(Abstract)
class PXII_API UPxiiListDataObjectBase : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModified, UPxiiListDataObjectBase*, EListDataModifyType);

	FOnListDataModified OnListDataModified;

	LIST_DATA_ACCESSOR(FName, DataID)
	LIST_DATA_ACCESSOR(FText, DataDisplayName)
	LIST_DATA_ACCESSOR(FText, DescriptionRichText)
	LIST_DATA_ACCESSOR(FText, DisabledRichText)
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage)
	LIST_DATA_ACCESSOR(UPxiiListDataObjectBase*, ParentListData)

	void InitDataObject();

	//Empty in base class. Child class will override this function to return the child data objects.
	virtual TArray<UPxiiListDataObjectBase*> GetAllChildListData() const { return TArray<UPxiiListDataObjectBase*>(); }
	virtual bool HasAnyChildListData() const { return false; }
	
	//The child class should override them to provide implementations for resetting the data
	virtual bool HasDefaultValue() const { return false; }
	virtual bool CanResetBackToDefaultValue() const { return false; }
	virtual bool TryResetBackToDefaultValue() { return false; }
	
	FORCEINLINE void SetShouldApplyChangeImmediately(bool InShouldApplyChangeImmediately) { bShouldApplyChangeImmediately = InShouldApplyChangeImmediately; };

protected:
	//Empty in base class. The child classes should override this function to implement their own initialization logic.
	virtual void OnDataObjectInitialized();

	virtual void NotifyListDataModified(UPxiiListDataObjectBase* InModifiedListData, EListDataModifyType InModifyReason = EListDataModifyType::DirectlyModified);

private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;

	UPROPERTY(Transient)
	UPxiiListDataObjectBase* ParentListData;
	
	bool bShouldApplyChangeImmediately = false;
};
