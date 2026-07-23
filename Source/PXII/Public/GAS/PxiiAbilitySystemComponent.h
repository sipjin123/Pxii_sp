// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Data/PxiiAbilityData.h"
#include "PxiiAbilitySystemComponent.generated.h"

class UPlayerInputSubsystem;

DECLARE_LOG_CATEGORY_EXTERN(LogTempAbilityComp, Log, All);
/**
 * 
 */
UCLASS(ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class PXII_API UPxiiAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable,Category="Ability")
	void GrantAbilityByRow(FName RowName);

	UFUNCTION(BlueprintCallable,Category="Ability")
	void GrantAllAbilities();

	UFUNCTION(BlueprintCallable,Category="Ability")
	void GrantAllPlayerEffects();

	UPROPERTY(EditAnywhere)
	bool LogAbilityInit;

	UFUNCTION(BlueprintPure)
	bool IsInputBlocked() const;

	UFUNCTION(BlueprintCallable, Category="Input")
	bool ConsumeBufferedInput(FGameplayTag InputTag);

	UFUNCTION(BlueprintCallable, Category="Input")
	bool ConsumeAndQueueInput(FGameplayTag InputTag);
	
	UFUNCTION(BlueprintPure, Category="Input")
	bool HasBufferedInput(FGameplayTag InputTag) const;

	UFUNCTION(BlueprintPure, Category="Input")
	bool CanQueueInput(FGameplayTag InputTag) const;

	bool AbilityInputTagPressed(const FGameplayTag& InputTag);
	
	void AbilityInputTagHeld(const FGameplayTag& InputTag);

	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	UFUNCTION()
	void HandleAbilityEnded(const FAbilityEndedData& EndData);

	UFUNCTION(BlueprintPure)
	FCooldownInfo GetCooldownRemainingForTag(FGameplayTag CooldownTag);
	
protected:
	UPxiiAbilitySystemComponent();
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Ability")
	TObjectPtr<UPxiiAbilityData> AbilityData;

private:
	UPlayerInputSubsystem* GetPlayerInputSubsystem() const;

	FGameplayTagContainer InputBlockTags;
};
