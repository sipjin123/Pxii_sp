// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Async/AbilityAsync.h"
#include "GAS/PxiiGameplayAbilityBase.h"
#include "WaitAbilityBlockedTagsAsync.generated.h"

// Because class inherits from UAbilityAsync, Unreal's K2Node compiler forces all 
// BlueprintAssignable delegates to share the exact same signature as the first delegate it finds.
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlockedTagAdded, FGameplayTag, AddedTag);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBlockedTagRemoved, FGameplayTag, RemovedTag, bool, bIsStillBlocked);

// Use generic one instead
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBlockedTagChanged, FGameplayTag, AddedOrRemovedTag, bool, bIsStillBlocked);
/**
 * 
 */
UCLASS()
class PXII_API UWaitAbilityBlockedTagsAsync : public UAbilityAsync
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "PXII|WaitAbilityBlockedTagsAsync", meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "true"))
	static UWaitAbilityBlockedTagsAsync* WaitForAbilityBlockedTags(
		AActor* TargetActor,
		TSubclassOf<UPxiiGameplayAbilityBase> AbilityClass,
		bool bOnlyTriggerOnce = false
	);
	
	// UPROPERTY(BlueprintAssignable, Category = "PXII|WaitAbilityBlockedTagsAsync")
	// FOnBlockedTagAdded OnBlockedTagAdded;
	//
	// UPROPERTY(BlueprintAssignable, Category = "PXII|WaitAbilityBlockedTagsAsync")
	// FOnBlockedTagRemoved OnBlockedTagRemoved;
	
	UPROPERTY(BlueprintAssignable, Category = "PXII|WaitAbilityBlockedTagsAsync")
	FOnBlockedTagChanged OnBlockedTagAdded;
	
	UPROPERTY(BlueprintAssignable, Category = "PXII|WaitAbilityBlockedTagsAsync")
	FOnBlockedTagChanged OnBlockedTagRemoved;
	
	virtual void Activate() override;
	virtual void EndAction() override;
	
protected:
	void OnTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
private:
	TSubclassOf<UPxiiGameplayAbilityBase> CachedAbilityClass;
	FGameplayTagContainer CachedBlockedTags;
	bool bOnlyTriggerOnce = false;
	
	TMap<FGameplayTag, FDelegateHandle> RegisteredTagHandles;
};
