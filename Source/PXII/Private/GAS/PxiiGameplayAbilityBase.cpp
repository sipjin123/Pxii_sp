// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PxiiGameplayAbilityBase.h"
#include "AbilitySystemComponent.h"
#include "GAS/PxiiAttributeSet.h"
#include "GAS/PxiiGameplayEffectCooldown.h"
#include "GAS/PxiiGameplayEffectCost.h"
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
		if (LogData)
			PXII_LOG(ELogCategory::Ability, Log, TEXT("DZ_LOG:: Cooldown found using custom CD Class"));
		FGameplayEffectSpecHandle specHandle = MakeOutgoingGameplayEffectSpec(CooldownGameplayEffectClass, GetAbilityLevel());

		if(specHandle.IsValid())
		{
			//specHandle.Data->DynamicGrantedTags.AddTag(CooldownTag); // add this line
			ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, specHandle);
		}
	}
	else
	{
		if(!CooldownTag.IsValid())
		{
			return;
		}
		
		if (LogData)
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

FAbilityCost UPxiiGameplayAbilityBase::CalculateCost() const
{
	return AbilityCost;
}

bool UPxiiGameplayAbilityBase::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;
	}

	const UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
	{
		return false;
	}

	const FAbilityCost Cost = CalculateCost();
	const float CurrentMana = ASC->GetNumericAttribute(UPxiiAttributeSet::GetManaAttribute());
	const float CurrentStamina = ASC->GetNumericAttribute(UPxiiAttributeSet::GetStaminaAttribute());
	const float CurrentHealth =	ASC->GetNumericAttribute(UPxiiAttributeSet::GetHealthAttribute());
	const float CurrentYin =	ASC->GetNumericAttribute(UPxiiAttributeSet::GetYinAttribute());
	const float CurrentYang =	ASC->GetNumericAttribute(UPxiiAttributeSet::GetYangAttribute());

	return CheckCostData(Cost.Mana, CurrentMana, "Mana") &&
 		   CheckCostData(Cost.Stamina, CurrentStamina, "Stamina") &&
 		   CheckCostData(Cost.Health, CurrentHealth, "Health") &&
 		   CheckCostData(Cost.Yin, CurrentYin, "Yin") &&
 		   CheckCostData(Cost.Yang, CurrentYang, "Yang"); 
}

bool UPxiiGameplayAbilityBase::CheckCostData(const FCostData& CostData, float currentValue, FString attrib) const
{
	bool result = false;
	if(CostData.Value <= 0)
	{
		return true;		
	}

	if(currentValue <= 0)
	{
		result = CostData.Value <= 0.0f;
		PXII_LOG(ELogCategory::Ability, Log, TEXT("DZ_LOG:: ATTRIB: %s Current value is 0! RESULT: %s"),*attrib, result ? TEXT("PASS") : TEXT("FAIL"));
		return result;
	}

	if(CostData.IsPercentage)
	{
		PXII_LOG(ELogCategory::Ability, Log, TEXT("DZ_LOG:: ATTRIB: %s PERCENTAGE! RESULT: PASS"), *attrib);
		return true;
	}

	result = CostData.Value < currentValue;
	PXII_LOG(ELogCategory::Ability, Log, TEXT("DZ_LOG::ATTRIB: %s FINAL RESULT: %s"),*attrib, result ? TEXT("PASS") : TEXT("FAIL"));

	return CostData.Value < currentValue;
}

void UPxiiGameplayAbilityBase::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

	if(!AbilityCost.HasAnyCost())
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (!ASC)
	{
		return;
	}

	TSubclassOf<UGameplayEffect> CostGameplayEffect = UPxiiGameplayEffectCost::StaticClass();
	if(!CostGameplayEffectClass)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CostGameplayEffect, GetAbilityLevel());

		if (!SpecHandle.IsValid())
		{
			return;
		}

		float manaCost = GetFinalCost(AbilityCost.Mana, ASC->GetNumericAttribute(UPxiiAttributeSet::GetManaAttribute()));
		float staminaCost = GetFinalCost(AbilityCost.Stamina, ASC->GetNumericAttribute(UPxiiAttributeSet::GetManaAttribute()));
		float healthCost = GetFinalCost(AbilityCost.Health, ASC->GetNumericAttribute(UPxiiAttributeSet::GetManaAttribute()));
		float yinCost = GetFinalCost(AbilityCost.Yin, ASC->GetNumericAttribute(UPxiiAttributeSet::GetManaAttribute()));
		float yangCost = GetFinalCost(AbilityCost.Yang, ASC->GetNumericAttribute(UPxiiAttributeSet::GetManaAttribute()));

		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Magnitude.Cost.Health"), -healthCost);
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Magnitude.Cost.Mana"), -manaCost);
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Magnitude.Cost.Stamina"), -staminaCost);
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Magnitude.Cost.Yang"), -yangCost);
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Magnitude.Cost.Yin"), -yinCost);

		ApplyGameplayEffectSpecToOwner(Handle,	ActorInfo,	ActivationInfo,	SpecHandle);
	}
	else
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CostGameplayEffectClass, GetAbilityLevel());
		if (!SpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(Handle,	ActorInfo,	ActivationInfo,	SpecHandle);
		}
	}
}

float UPxiiGameplayAbilityBase::GetFinalCost(const FCostData& costData, float currentValue) const
{
	if(costData.IsPercentage)
	{
		return currentValue * costData.Value;
	}

	return costData.Value;
}
