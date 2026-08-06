// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PxiiCharacter.h"
#include "GAS/PxiiGA_OneshotBase.h"
#include "PxiiGA_Fire.generated.h"

/**
 * 
 */
DECLARE_LOG_CATEGORY_EXTERN(LogFireProjectile, Log, All);
UCLASS()
class PXII_API UPxiiGA_Fire : public UPxiiGA_OneshotBase
{
	GENERATED_BODY()
	
public:
	UPxiiGA_Fire();

	// Override the activation function
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RequestProjectile(const FVector& projectilDir);

	void RequestProjectile_Implementation(const FVector& projectilDir);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FireProjectile(APxiiCharacter* Character);

	void FireProjectile_Implementation(APxiiCharacter* Character);
	
	
private :

	UPROPERTY()
	APxiiCharacter* SelfCharacter;
	
	UPROPERTY(EditAnywhere)
	float DrawDuration = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta=(AllowPrivateAccess = true))
	bool TraceProcessDamage = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta=(AllowPrivateAccess = true))
	FName MuzzleSocketName;
};
