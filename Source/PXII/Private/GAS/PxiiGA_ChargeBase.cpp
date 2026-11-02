// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PxiiGA_ChargeBase.h"

#include "Utility/PxiiCombatBPLibrary.h"

void UPxiiGA_ChargeBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UPxiiGA_ChargeBase::OnHoldReleased_Implementation(float TimeHeld)
{
	Super::OnHoldReleased_Implementation(TimeHeld);

	if(TimeHeld >= ChargeTime)
	{
		OnChargeSuccess();
	}
	else
	{
		OnChargeFailed();
	}
}

void UPxiiGA_ChargeBase::RequestProjectile_Implementation(const FHitInformation& projectilDir)
{

}

void UPxiiGA_ChargeBase::FireProjectile_Implementation(APxiiCharacter* Character)
{
	FHitInformation TraceInformation;
	UPxiiCombatBPLibrary::StartProjectileTrace(SelfCharacter, TraceInformation, TraceProcessDamage,   MuzzleSocketName, DrawDebugTrace);
	RequestProjectile(TraceInformation);
}

void UPxiiGA_ChargeBase::OnChargeSuccess_Implementation()
{
	
}

void UPxiiGA_ChargeBase::OnChargeFailed_Implementation()
{
	
}
