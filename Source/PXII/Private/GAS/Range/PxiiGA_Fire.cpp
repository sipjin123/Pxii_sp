// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Range/PxiiGA_Fire.h"
#include "AbilitySystemComponent.h"
#include "Character/PxiiCharacter.h"
#include "Components/PxiiCombatComponent.h"
#include "Components/PxiiPlayerCombatComponent.h"
#include "Enum/PxiiDamageType.h"
#include "GAS/PxiiAttributeSet.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Subsystem/WorldSpawnerSubsystem.h"
#include "Utility/PxiiCombatBPLibrary.h"
#include "Utility/PxiiDebugTraceBPLibrary.h"

DEFINE_LOG_CATEGORY(LogFireProjectile);
UPxiiGA_Fire::UPxiiGA_Fire()
{
    ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Pxii.Weapon.Type.Range.OneShot")));
    ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Pxii.Combat.State.ADS")));
}

void UPxiiGA_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AActor* Avatar = ActorInfo->AvatarActor.Get();
    
	SelfCharacter = Cast<APxiiCharacter>(Avatar);
	if (SelfCharacter && SelfCharacter->Implements<UPxiiCombatInterface>())
	{
		APxiiWeaponRange* WeaponRef = IPxiiCombatInterface::Execute_GetWeaponBaseRange(SelfCharacter);
		if (WeaponRef)
		{
			UPxiiGameplayEffectCooldown* GECooldownRef = WeaponRef->GEFireRateCooldown;
			// Note(Burlin): Maybe add scaling cooldown here in the future
		}
		else
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
    
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}

	    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		FireProjectile(SelfCharacter);
	}
    
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UPxiiGA_Fire::FireProjectile_Implementation(APxiiCharacter* Character)
{
	FHitInformation TraceInformation;
    UPxiiCombatBPLibrary::StartProjectileTrace(SelfCharacter, TraceInformation, TraceProcessDamage,   MuzzleSocketName, DrawDebugTrace);
	RequestProjectile(TraceInformation);
}

void UPxiiGA_Fire::RequestProjectile_Implementation(const FHitInformation& projectilDir)
{
	
}




