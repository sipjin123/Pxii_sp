// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/PxiiGameplayAbilityHold.h"
#include "Character/PxiiCharacter.h"
#include "GAS/PxiiGameplayAbilityBase.h"
#include "PxiiGA_ADS.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiGA_ADS : public UPxiiGameplayAbilityHold
{
	GENERATED_BODY()
public:
	explicit UPxiiGA_ADS(const FObjectInitializer& ObjectInitializer);

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
private:

	UPROPERTY()
	TObjectPtr<APxiiCharacter> CurrentPlayerCharacter;

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float OffsetVerticalAdjustment = 2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float OffsetHorizontalAdjustment = 0.f;

};
