// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Async/AbilityAsync.h"
#include "GAS/PxiiGameplayAbilityBase.h"
#include "WaitAbilityRequiredTagsAsync.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRequiredTagChanged, FGameplayTag, ChangedTag, FGameplayTagContainer, MissingTags);
/**
 * 
 */
UCLASS()
class PXII_API UWaitAbilityRequiredTagsAsync : public UAbilityAsync
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "PXII|WaitAbilityRequiredTagsAsync", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "true"))
	static UWaitAbilityRequiredTagsAsync* WaitForAbilityRequiredTags(
		UObject* WorldContextObject,
		AActor* TargetActor,
		TSubclassOf<UPxiiGameplayAbilityBase> AbilityClass,
		bool bOnlyTriggerOnce = false
	);
	
	UPROPERTY(BlueprintAssignable, Category = "PXII|WaitAbilityRequiredTagsAsync")
	FOnRequiredTagChanged OnRequiredTagsMet;
	
	UPROPERTY(BlueprintAssignable, Category = "PXII|WaitAbilityRequiredTagsAsync")
	FOnRequiredTagChanged OnRequiredTagMissing;
	
	virtual void Activate() override;
	virtual void EndAction() override;
	
protected:
	void OnTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	FGameplayTagContainer GetMissingRequiredTags(class UAbilitySystemComponent* ASC) const;
	
private:
	TSubclassOf<UPxiiGameplayAbilityBase> CachedAbilityClass;
	FGameplayTagContainer CachedRequiredTags;
	bool bOnlyTriggerOnce = false;
	bool bIsCurrentTagsMet = false;
	
	TMap<FGameplayTag, FDelegateHandle> RegisteredTagHandles;
};
