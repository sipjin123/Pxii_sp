// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PxiiGameplayAbilityBase.h"

#include "GAS/PxiiGameplayEffectBase.h"
#include "GAS/PxiiGameplayEffectCooldown.h"
#include "Utility/PXIILogUtility.h"


UPxiiGameplayAbilityBase::UPxiiGameplayAbilityBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Pxii.Input.Mode.UI")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("InputMode.Menu")));
}

UMoverComponent* UPxiiGameplayAbilityBase::GetMoverComponent()
{
	AActor* Actor = GetAvatarActorFromActorInfo();
	UMoverComponent* Mover = Actor->FindComponentByClass<UMoverComponent>();

	return Mover;
}

const FGameplayTagContainer* UPxiiGameplayAbilityBase::GetCooldownTags() const
{
	TempCooldownTags.Reset();
	
	if(CooldownTag.IsValid())
	{
		TempCooldownTags.AddTag(CooldownTag);
	}

	return &TempCooldownTags;
}

void UPxiiGameplayAbilityBase::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	TSubclassOf<UGameplayEffect> CooldownGE = UPxiiGameplayEffectCooldown::StaticClass();
	if(CooldownGameplayEffectClass)
	{
		CooldownGE = CooldownGameplayEffectClass;
		FGameplayEffectSpecHandle specHandle = MakeOutgoingGameplayEffectSpec(CooldownGE, GetAbilityLevel());

		if(specHandle.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, specHandle);
		}
	}
	else
	{
		if(!CooldownTag.IsValid())
		{
			return;
		}
		
		PXII_LOG(ELogCategory::Ability, Log, TEXT("DZ_LOG:: Cooldown Ability is empty using default cooldown class"));

		FGameplayEffectSpecHandle specHandle = MakeOutgoingGameplayEffectSpec(CooldownGE, GetAbilityLevel());

		if(specHandle.IsValid())
		{
			specHandle.Data->DynamicGrantedTags.Reset();
			specHandle.Data->DynamicGrantedTags.AddTag(CooldownTag);
			specHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Magnitude.Data.Cooldown"),	CooldownDuration);
			ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, specHandle);
		}
	}
}
