// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonNumericTextBlock.h"
#include "Engine/DataAsset.h"
#include "StructUtils/InstancedStruct.h"
#include "PxiiOptionsData.generated.h"

UENUM(BlueprintType)
enum class EOptionsDataType : uint8
{
	String,
	Boolean,
	Slider,
};

USTRUCT(BlueprintType)
struct FOptionsMap
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ForceInlineRow))
	FText Value;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ForceInlineRow))
	FText DisplayName;
};

USTRUCT()
struct FDataBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FStringData : public FDataBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "String Data")
	FText DataID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "String Data")
	FText DataDisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "String Data")
	FText DescriptionRichText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "String Data")
	TArray<FOptionsMap> OptionsSet;
};

USTRUCT(BlueprintType)
struct FBooleanData : public FDataBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FSliderData : public FDataBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slider Data")
	FText DataID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slider Data")
	FText DataDisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slider Data")
	FText DescriptionRichText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slider Data", meta = (ForceInlineRow))
	FVector2D DisplayValueRange { 0, 1.f };
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slider Data", meta = (ForceInlineRow))
	FVector2D OutputValueRange { 0, 2.f };
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slider Data", meta = (ToolTip = "The precision of the slider"))
	float SliderStepSize { 0.01f };
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slider Data")
	ECommonNumericType DisplayNumericType = ECommonNumericType::Percentage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slider Data")
	bool bShowDecimal { false };
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slider Data", meta = (EditCondition = "bShowDecimal", ToolTip = "How many decimals should show?"))
	int32 FractionalDigits { 1 };
};

USTRUCT(BlueprintType)
struct FOptionsDataEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EOptionsDataType Type = EOptionsDataType::String;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BaseStruct = "/Script/PXII.DataBase", ExcludeBaseStruct))
	FInstancedStruct Data;
};

USTRUCT(BlueprintType)
struct FSubOptionsDataEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sub Category")
	FText CategoryID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sub Category")
	FText CategoryDisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Options Data")
	TArray<FOptionsDataEntry> DataToConstruct;
};

USTRUCT(BlueprintType)
struct FTabsEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Options Data")
	FText TabName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Options Data")
	bool bSubCategory { false };
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Options Data", meta = (ToolTip = "No sub-category needed", EditCondition = "!bSubCategory", EditConditionHides))
	TArray<FOptionsDataEntry> DataToConstruct;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Options Data", meta = (ToolTip = "Sub-category needed", EditCondition = "bSubCategory", EditConditionHides))
	TArray<FSubOptionsDataEntry> SubDataToConstruct;
};

/**
 * 
 */
UCLASS()
class PXII_API UPxiiOptionsData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Options Tab")
	TArray<FTabsEntry> TabsToConstruct;
};
