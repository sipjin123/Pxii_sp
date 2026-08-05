// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CommonUI/PxiiActivatableWidget.h"
#include "Engine/DataAsset.h"
#include "PxiiUIClassesMapping.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiUIClassesMapping : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "UI | UI Classes Soft Reference", meta = (ForceInlineRow, Categories = "Pxii.UI.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UPxiiActivatableWidget>> UIClassesMap;
};
