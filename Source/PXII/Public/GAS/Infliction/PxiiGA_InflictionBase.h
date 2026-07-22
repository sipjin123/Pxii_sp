// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/PxiiGA_OneshotBase.h"
#include "PxiiGA_InflictionBase.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiGA_InflictionBase : public UPxiiGA_OneshotBase
{
	GENERATED_BODY()
public:
	UPxiiGA_InflictionBase();

	UFUNCTION()
	virtual void OnCompleted();

	UFUNCTION()
	virtual void OnBlendOut();

	UFUNCTION()
	virtual void OnInterrupted();

	UFUNCTION()
	virtual void OnCancelled();

	UFUNCTION(BlueprintImplementableEvent, Category = "PXII|Ability|Dodge", DisplayName = "OnCompleted", meta = (ScriptName = "OnCompleted"))
	void K2_OnCompleted();							   
													   
	UFUNCTION(BlueprintImplementableEvent, Category = "PXII|Ability|Dodge", DisplayName = "OnBlendOut", meta = (ScriptName = "OnBlendOut"))
	void K2_OnBlendOut();							   
													   
	UFUNCTION(BlueprintImplementableEvent, Category = "PXII|Ability|Dodge", DisplayName = "OnInterrupted", meta = (ScriptName = "OnInterrupted"))
	void K2_OnInterrupted();						   
													   
	UFUNCTION(BlueprintImplementableEvent, Category = "PXII|Ability|Dodge", DisplayName = "OnCancelled", meta = (ScriptName = "OnCancelled"))
	void K2_OnCancelled();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> MontageToPlay;
};
