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
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void RequestProjectileHit(APxiiCharacter* PxiiCharacter, const FVector& Vector, const FVector& ImpactPoint);
	UFUNCTION(BlueprintCallable)
	void FireProjectile(APxiiCharacter* Character);

	// The name of the socket on the weapon's mesh from which the projectile should spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FName MuzzleSocketName;

	UPROPERTY()
	APxiiCharacter* SelfCharacter;
public:
};
