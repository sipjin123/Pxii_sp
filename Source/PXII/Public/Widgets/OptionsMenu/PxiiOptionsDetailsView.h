// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "CommonTextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/Components/DataObject/PxiiListDataObjectBase.h"
#include "PxiiOptionsDetailsView.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class PXII_API UPxiiOptionsDetailsView : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateDetailsInfo(UPxiiListDataObjectBase* InDataObject, const FString& InEntryWidgetClassName = FString());
	void ClearDetailsInfo();
	
protected:
	//~ Begin UUserWidget Interface
	void NativeOnInitialized() override;
	//~ End UUserWidget Interface
	
private:
#pragma region Bound Widgets
	UPROPERTY(meta = (BindWidget))
	UCommonTextBlock* TextBlock_Title;
	
	UPROPERTY(meta = (BindWidget))
	UCommonLazyImage* Image_DescriptionImage;
	
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* RichTextBlock_Description;
	
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* RichTextBlock_DynamicDetails;
	
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* RichTextBlock_DisabledReason;
#pragma endregion 
};
