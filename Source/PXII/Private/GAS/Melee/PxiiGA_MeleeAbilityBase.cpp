// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Melee/PxiiGA_MeleeAbilityBase.h"

UPxiiGA_MeleeAbilityBase::UPxiiGA_MeleeAbilityBase(const FObjectInitializer& ObjectInitializer)
{
}

#pragma region Combat

void UPxiiGA_MeleeAbilityBase::OnCompleted()
{
	K2_OnCompleted();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPxiiGA_MeleeAbilityBase::OnBlendOut()
{
	K2_OnBlendOut();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UPxiiGA_MeleeAbilityBase::OnInterrupted()
{
	K2_OnInterrupted();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UPxiiGA_MeleeAbilityBase::OnCancelled()
{
	K2_OnCancelled();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

#pragma endregion