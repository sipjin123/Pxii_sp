// Fill out your copyright notice in the Description page of Project Settings.
#include "GAS/PxiiGameplayEffectCost.h"

#include "GAS/PxiiAttributeSet.h"

UPxiiGameplayEffectCost::UPxiiGameplayEffectCost()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FSetByCallerFloat ManaSetByCaller;
	ManaSetByCaller.DataTag = FGameplayTag::RequestGameplayTag("Magnitude.Cost.Mana");

	FSetByCallerFloat StaminaSetByCaller;
	StaminaSetByCaller.DataTag = FGameplayTag::RequestGameplayTag("Magnitude.Cost.Stamina");

	FSetByCallerFloat HealthSetByCaller;
	HealthSetByCaller.DataTag = FGameplayTag::RequestGameplayTag("Magnitude.Cost.Health");

	FSetByCallerFloat YinSetByCaller;
	YinSetByCaller.DataTag = FGameplayTag::RequestGameplayTag("Magnitude.Cost.Yin");

	FSetByCallerFloat YangSetByCaller;
	YangSetByCaller.DataTag = FGameplayTag::RequestGameplayTag("Magnitude.Cost.Yang");

	
	FGameplayModifierInfo ManaMod;
	ManaMod.Attribute = UPxiiAttributeSet::GetManaAttribute();
	ManaMod.ModifierOp = EGameplayModOp::Additive;
	ManaMod.ModifierMagnitude = FGameplayEffectModifierMagnitude(ManaSetByCaller);

	FGameplayModifierInfo StaminaMod;
	StaminaMod.Attribute = UPxiiAttributeSet::GetStaminaAttribute();
	StaminaMod.ModifierOp = EGameplayModOp::Additive;
	StaminaMod.ModifierMagnitude = FGameplayEffectModifierMagnitude(StaminaSetByCaller);

	FGameplayModifierInfo HealthMod;
	HealthMod.Attribute = UPxiiAttributeSet::GetHealthAttribute();
	HealthMod.ModifierOp = EGameplayModOp::Additive;
	HealthMod.ModifierMagnitude = FGameplayEffectModifierMagnitude(HealthSetByCaller);

	FGameplayModifierInfo YinMod;
	YinMod.Attribute = UPxiiAttributeSet::GetYinAttribute();
	YinMod.ModifierOp = EGameplayModOp::Additive;
	YinMod.ModifierMagnitude = FGameplayEffectModifierMagnitude(YinSetByCaller);

	FGameplayModifierInfo YangMod;
	YangMod.Attribute = UPxiiAttributeSet::GetYangAttribute();
	YangMod.ModifierOp = EGameplayModOp::Additive;
	YangMod.ModifierMagnitude = FGameplayEffectModifierMagnitude(YangSetByCaller);

	Modifiers.Reset();
	Modifiers.Add(ManaMod);
	Modifiers.Add(StaminaMod);
	Modifiers.Add(HealthMod);
	Modifiers.Add(YinMod);
	Modifiers.Add(YangMod);
}
