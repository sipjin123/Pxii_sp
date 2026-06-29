// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/Object.h"
#include "PxiiCombatInterface.generated.h"

/**
 * 
 */

UINTERFACE(MinimalAPI, BlueprintType)
class UPxiiCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class PXII_API IPxiiCombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	UPxiiAbilitySystemComponent* GetPxiiAbilitySystemComponent()const;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	UPxiiCombatComponent* GetCombatComponent()const;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	UPxiiAttributeSet* GetAttributeSet()const;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	bool IsAlive()const;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	bool CanReceiveDamage()const;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	bool IsInvincible()const;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	bool IsFriendly(const AActor* Other)const;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	AActor* GetCombatTarget()const;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	void SetCombatTarget(AActor* NewTarget);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	FVector GetCombatSocketLocation(FName SocketName)const;

	//UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	//bool ApplyDamage(const FPxiiDamageData& DamageData);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	void OnDeath();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Combat")
	void Revive();
};
