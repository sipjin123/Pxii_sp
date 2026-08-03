// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MoverComponent.h"
#include "PxiiGameplayAbilityBase.generated.h"

USTRUCT(BlueprintType)
struct FCostData
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsPercentage;
};

USTRUCT(BlueprintType)
struct PXII_API FAbilityCost
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCostData Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCostData Mana;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCostData Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCostData Yin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCostData Yang;

	bool HasAnyCost() const
	{
		return Health.Value > 0 ||
			Mana.Value > 0 ||
			Stamina.Value > 0 ||
			Yin.Value > 0 ||
			Yang.Value > 0;
	}
};

USTRUCT(BlueprintType)
struct PXII_API FCooldownInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float RemainingTime = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float TotalDuration = 0.f;
};

/**
 * 
 */
UCLASS()
class PXII_API UPxiiGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	
	UPxiiGameplayAbilityBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cooldown")
	FAbilityCost AbilityCost;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cooldown")
	FGameplayTag CooldownTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cooldown")
	float CooldownDuration = 1.5f;
	
	UFUNCTION(BlueprintPure)
	UMoverComponent* GetMoverComponent();

	virtual const FGameplayTagContainer* GetCooldownTags() const override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	FAbilityCost CalculateCost() const;
	
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual bool CheckCostData(const FCostData& CostData, float currentValue, FString attrib) const;

	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	float GetFinalCost(const FCostData& costData, float currentValue) const;

private:
	mutable FGameplayTagContainer TempCooldownTags;
public:
	UPROPERTY(EditAnywhere)
	bool LogData;
};
