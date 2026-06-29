// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "PxiiAbilityData.generated.h"

class UGameplayAbility;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPxiiAbilityData:public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(MultiLine=true))
	FText Description;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 Level=1;
};