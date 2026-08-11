// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTextBlock.h"
#include "PxiiLoadingBarStyle.h"
#include "Engine/DataTable.h"
#include "Widgets/Layout/SScaleBox.h"
#include "PxiiLoadingScreenTableRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct LOADINGSCREENMODULE_API FPxiiLoadingScreenTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Loading | Background")
	TArray<TSoftObjectPtr<UTexture2D>> BackgroundTextures;
	
	UPROPERTY(EditAnywhere, Category = "Loading | Background")
	TEnumAsByte<EStretch::Type> BackgroundStretchMode = EStretch::ScaleToFill;
	
	UPROPERTY(EditAnywhere, Category = "Loading | Background")
	bool bRandomizeBackground = true;
	
	UPROPERTY(EditAnywhere, Category = "Loading | Message")
	TSoftObjectPtr<UStringTable> MessageStringTable;
	
	UPROPERTY(EditAnywhere, Category = "Loading | Message", meta = (ClampMin = "0.5"))
	float MessageCycleInterval = 4.f;
	
	UPROPERTY(EditAnywhere, Category = "Loading | Message")
	bool bShuffleMessage = true;
	
	UPROPERTY(EditAnywhere, Category = "Loading | Message")
	TSubclassOf<UCommonTextStyle> MessageTextStyleClass;
	
	UPROPERTY(EditAnywhere, Category = "Loading | Progress Bar")
	TSubclassOf<UCommonTextStyle> ProgressBarTextStyleClass;
	
	UPROPERTY(EditAnywhere, Category = "Loading | Progress Bar")
	TSoftObjectPtr<UPxiiLoadingBarStyle> ProgressBarStyleAsset;
	
	UPROPERTY(EditAnywhere, Category = "Loading | Time", meta = (ClampMin = "0.0"))
	float MinimumDisplayTime = 2.f;
};
