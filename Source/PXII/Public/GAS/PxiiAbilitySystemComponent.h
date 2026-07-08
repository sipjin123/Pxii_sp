// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
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

	UPROPERTY(EditAnywhere)
	bool LogAbilityInit;

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
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Ability")
	UDataTable* AbilityDataTable;

private:
	UPlayerInputSubsystem* GetPlayerInputSubsystem() const;
};
