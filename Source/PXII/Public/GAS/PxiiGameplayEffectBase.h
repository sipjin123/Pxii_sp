// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "PxiiGameplayEffectBase.generated.h"

UENUM(BlueprintType)
enum class EEffectTextColor : uint8
{
	Custom,
	Buff,
	Debuff,
};

USTRUCT(BlueprintType)
struct FEffectTextCustomization
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText EffectDisplayText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEffectTextColor EffectColorType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSlateColor EffectColor;
};

USTRUCT(BlueprintType)
struct FEffectDetails
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool ShowDisplayText;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FEffectTextCustomization> EffectDisplayText;
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class PXII_API UPxiiGameplayEffectBase : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UPxiiGameplayEffectBase();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bShowFloatingText = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FEffectDetails EffectDetails;
};
