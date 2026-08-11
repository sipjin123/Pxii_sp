// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "PxiiLoadingBarStyle.generated.h"

/**
 * 
 */
UCLASS()
class LOADINGSCREENMODULE_API UPxiiLoadingBarStyle : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Loading | Progress Bar")
	FProgressBarStyle ProgressBarStyle;
};
