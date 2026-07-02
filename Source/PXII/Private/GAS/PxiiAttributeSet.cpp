// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PxiiAttributeSet.h"

UPxiiAttributeSet::UPxiiAttributeSet()
{
	Health.SetBaseValue(100.0f);
	Health.SetCurrentValue(100.0f);
	MaxHealth.SetBaseValue(100.0f);
	MaxHealth.SetCurrentValue(100.0f);
	Mana.SetBaseValue(100.0f);
	MaxMana.SetCurrentValue(100.0f);
	Ammo.SetBaseValue(30.0f);
	Ammo.SetCurrentValue(30.0f);
}

void UPxiiAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UPxiiAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
