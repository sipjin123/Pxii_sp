// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "PxiiGameLevelMapping.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiGameLevelMapping : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Game | Game Levels Soft Reference", meta = (ForceInlineRow, Categories = "Pxii.Level"))
	TMap<FGameplayTag, TSoftObjectPtr<UWorld>> GameLevelMapping;
};
