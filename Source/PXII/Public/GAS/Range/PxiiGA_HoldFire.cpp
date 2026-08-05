#include "PxiiGA_HoldFire.h"

#include "Utility/PxiiCombatBPLibrary.h"

UPxiiGA_HoldFire::UPxiiGA_HoldFire()
{
	ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Pxii.Weapon.Type.Range.HoldToShoot")));
	ActivationRequiredTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Pxii.Combat.State.ADS")));
}

void UPxiiGA_HoldFire::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	AActor* Avatar = ActorInfo->AvatarActor.Get();
    
	APxiiCharacter* SelfCharacter = Cast<APxiiCharacter>(Avatar);
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
	}
}

void UPxiiGA_HoldFire::FireProjectile_Implementation(APxiiCharacter* Character)
{
	FVector TraceDir;
	UPxiiCombatBPLibrary::StartProjectileTrace(Character, TraceDir, MuzzleSocketName);
	RequestProjectile(TraceDir);
}

void UPxiiGA_HoldFire::RequestProjectile_Implementation(const FVector& projectilDir)
{
}
