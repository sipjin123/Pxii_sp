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
	
	MovementSpeed.SetBaseValue(30.0f);
	MovementSpeed.SetCurrentValue(30.0f);
	BaseMovementSpeed.SetBaseValue(30.0f);
	BaseMovementSpeed.SetCurrentValue(30.0f);
	MovementSpeedBonus.SetBaseValue(0.0f);
	MovementSpeedBonus.SetCurrentValue(0.0f);
	SpeedModifier.SetBaseValue(1.0f);
	SpeedModifier.SetCurrentValue(1.0f);

	Yin.SetBaseValue(0.0f);
	MaxYin.SetBaseValue(100.0f);
	Yang.SetBaseValue(0.0f);
	MaxYang.SetBaseValue(100.0f);
	
	StaggerMeter.SetBaseValue(0.0f);
	MaxStaggerMeter.SetBaseValue(100.0f);
	Shield.SetBaseValue(0.0f);
	MaxShield.SetBaseValue(100.0f);
	
	Defense.SetBaseValue(10.0f);
	AttackDamage.SetBaseValue(10.0f);
}

void UPxiiAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UPxiiAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

