// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "PxiiAbilityData.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAbilityData
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AbilityID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,meta=(MultiLine=true))
	FText Description;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag InputTag;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 Level=1;
};

USTRUCT(BlueprintType)
struct FGameplayEffectData
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName EffectID;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,meta=(MultiLine=true))
	FText Description;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EffectClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	int32 Level=1;
};

/**
 * 
 */
UCLASS(BlueprintType)
class PXII_API UPxiiAbilityData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FAbilityData> PlayerGrantedAbilities;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FGameplayEffectData> PlayerGrantedEffect;
};