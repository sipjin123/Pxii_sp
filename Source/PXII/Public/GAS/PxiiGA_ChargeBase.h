// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PxiiGameplayAbilityHold.h"
#include "GAS/PxiiGameplayAbilityBase.h"
#include "PxiiGA_ChargeBase.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiGA_ChargeBase : public UPxiiGameplayAbilityHold
{
	GENERATED_BODY()
	
public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnHoldReleased_Implementation(float TimeHeld) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RequestProjectile(const FHitInformation& projectilDir);

	void RequestProjectile_Implementation(const FHitInformation& projectilDir);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FireProjectile(APxiiCharacter* Character);

	void FireProjectile_Implementation(APxiiCharacter* Character);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnChargeSuccess();

	void OnChargeSuccess_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnChargeFailed();

	void OnChargeFailed_Implementation();

private:
	
	// UPROPERTY()
	// FGameplayAbilitySpecHandle AbilityHandle;
	//
	// UPROPERTY()
	// TObjectPtr<FGameplayAbilityActorInfo> AbilityActorInfo;
	//
	// UPROPERTY()
	// FGameplayAbilityActivationInfo AbilityActivationInfo;
	//
	// UPROPERTY()
	// FGameplayEventData* AbilityTriggerEventData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta=(AllowPrivateAccess = true))
	bool DrawDebugTrace = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta=(AllowPrivateAccess = true))
	bool TraceProcessDamage = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta=(AllowPrivateAccess = true))
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	float ChargeTime;
};
