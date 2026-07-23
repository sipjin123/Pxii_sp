// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MoverComponent.h"
#include "PxiiGameplayAbilityBase.generated.h"

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
	FGameplayTag CooldownTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Cooldown")
	float CooldownDuration = 1.5f;
	
	UFUNCTION(BlueprintPure)
	UMoverComponent* GetMoverComponent();

	virtual const FGameplayTagContainer* GetCooldownTags() const override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

private:

	mutable FGameplayTagContainer TempCooldownTags;
};
