// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Melee/PxiiGA_MeleeAttack.h"
#include "Character/PxiiCharacter.h"

bool UPxiiGA_MeleeAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                             const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	/*
	//auto* Character = CastChecked<APxiiCharacter>(ActorInfo->AvatarActor.Get());
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		
		UE_LOG(LogTemp, Warning, TEXT("MeleeAttackCPP X"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("MeleeAttackCPP Y"));
	return true;*/
}

void UPxiiGA_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	/*
	if (!CommitAbility(Handle, OwnerInfo, ActivationInfo))
	{
		return;
	}*/
	
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);
}

void UPxiiGA_MeleeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#pragma region Combat

UPxiiGA_MeleeAttack::UPxiiGA_MeleeAttack(const FObjectInitializer& ObjectInitializer)
{
	ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Pxii.Weapon.Type.Melee")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Pxii.Combat.State.ADS")));
}

void UPxiiGA_MeleeAttack::OnCompleted()
{
	K2_OnCompleted();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPxiiGA_MeleeAttack::OnBlendOut()
{
	K2_OnBlendOut();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPxiiGA_MeleeAttack::OnInterrupted()
{
	K2_OnInterrupted();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UPxiiGA_MeleeAttack::OnCancelled()
{
	K2_OnCancelled();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

#pragma endregion