// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PxiiGameplayEffectCooldown.h"

UPxiiGameplayEffectCooldown::UPxiiGameplayEffectCooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat SetByCaller;
	SetByCaller.DataTag = FGameplayTag::RequestGameplayTag(TEXT("Magnitude.Data.Cooldown"));
	DurationMagnitude =	SetByCaller;
}
