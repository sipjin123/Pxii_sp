// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PxiiPlayerCombatComponent.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/PxiiPlayerImpactData.h"
#include "Data/PxiiTags.h"
#include "GAS/PxiiGameplayAbilityBase.h"
#include "Utility/PXIILogUtility.h"

UPxiiPlayerCombatComponent::UPxiiPlayerCombatComponent()
{
}

// Called when the game starts
void UPxiiPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	UPxiiAbilitySystemComponent* ASC = GetOwner()->GetComponentByClass<UPxiiAbilitySystemComponent>();

	if(!OwnerCharacter)
	{
		OwnerCharacter = Cast<APxiiCharacter>(GetOwner());
	}
	if(!ASC)
	{
		return;
	}

	ASC->AbilityActivatedCallbacks.AddUObject(this, &UPxiiPlayerCombatComponent::OnAbilityActivated);
	ASC->AbilityEndedCallbacks.AddUObject(this, &UPxiiPlayerCombatComponent::OnAbilityEnded);
}

void UPxiiPlayerCombatComponent::OnAbilityActivated(UGameplayAbility* ActivatedAbility)
{
	bool isAttackClass = false;

	UPxiiGameplayAbilityBase* base = Cast<UPxiiGameplayAbilityBase>(ActivatedAbility);
	if(base)
	{
		if(GetEffectDisplayComp())
		{
			if(base->EffectDetails.ShowDisplayText)
			{
				EffectDisplayComp->ShowAbilityText(base->EffectDetails, true);
			}
		}
	}
	
	for(auto attackClass : MeleeAttackClasses)
	{
		if(ActivatedAbility->GetClass()->IsChildOf(attackClass))
		{
			ActiveMeleeAbility += 1;
			isAttackClass = true;
			PendingAbilitiesToEnd.Add(ActivatedAbility);
			CheckActiveWeapon(EWeaponType::Armblade);
			break;
		}
	}

	if(!isAttackClass)
	{
		for(auto attackClass : RangeAttackClasses)
		{
			if(ActivatedAbility->GetClass()->IsChildOf(attackClass))
			{
				ActiveRangeAbility += 1;
				PendingAbilitiesToEnd.Add(ActivatedAbility);
				PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: ACTIVE ABILITY: %d"), ActiveRangeAbility);
				isAttackClass = true;
				CheckActiveWeapon(EWeaponType::Blaster);
				break;
			}
		}		
	}
	
	if(isAttackClass)
	{
		GetWorld()->GetTimerManager().ClearTimer(LastAttackTimer);
	}

}

void UPxiiPlayerCombatComponent::OnAbilityEnded(UGameplayAbility* ActivatedAbility)
{
	int reduction = 0;
	if(PendingAbilitiesToEnd.Contains(ActivatedAbility))
	{
		reduction = 1;
		PendingAbilitiesToEnd.Remove(ActivatedAbility);
	}
	
	float waitTime = 5.0f;
	if(ActiveWeapon == EWeaponType::Armblade)
	{
		
		ActiveMeleeAbility -= reduction;
		if(ActiveMeleeAbility > 0)
		{
			return;
		}
		waitTime = ArmbladeWeaponTimer;
	}
	else
	{
		ActiveRangeAbility -= reduction;
		PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: ACTIVE ABILITY: %d"), ActiveRangeAbility);
		if(ActiveRangeAbility > 0)
		{
			return;
		}
		waitTime = BlasterWeaponTimer;
	}
	
	GetWorld()->GetTimerManager().SetTimer(
		LastAttackTimer,
		this,
		&UPxiiPlayerCombatComponent::HideActiveWeapon,
		waitTime,
		false
	);
}

UPxiiEffectDisplayComponent* UPxiiPlayerCombatComponent::GetEffectDisplayComp()
{
	if(EffectDisplayComp)
	{
		return EffectDisplayComp;
	}
	EffectDisplayComp = GetOwner()->GetComponentByClass<UPxiiEffectDisplayComponent>();
	return EffectDisplayComp;
}

UCameraShakeAsset* UPxiiPlayerCombatComponent::GetCameraShakeImpact(EHitImpact hitImpact)
{
	if(PlayerImpactData == nullptr)
	{
		return nullptr;
	}

	auto CameraShake = PlayerImpactData->ImpactCameraShake;
	if(CameraShake.IsEmpty())
	{
		return nullptr;
	}

	if(!CameraShake.Contains(hitImpact))
	{
		return nullptr;
	}

	return CameraShake[hitImpact];
}

// Called every frame
void UPxiiPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPxiiPlayerCombatComponent::HideActiveWeapon_Implementation()
{
	if(ActiveWeapon == EWeaponType::Invalid)
	{
		return;
	}
	
	OwnerCharacter->CurrentWeaponMelee->HideWeapon();
	OwnerCharacter->CurrentWeaponRanged->HideWeapon();
	
	ActiveWeapon = EWeaponType::Invalid;
	OnWeaponActiveUpdated.Broadcast();
}

void UPxiiPlayerCombatComponent::CheckActiveWeapon(EWeaponType weaponType)
{
	if(ActiveWeapon == weaponType)
	{
		return;
	}

	if(weaponType == EWeaponType::Armblade)
	{
		PXII_LOG(ELogCategory::Ability, Log, TEXT("DZ_LOG:: Showing armblade"));

		ActiveWeapon = weaponType;
		OwnerCharacter->CurrentWeaponMelee->ShowWeapon();
		OwnerCharacter->CurrentWeaponRanged->HideWeapon();
	}
	else if(weaponType == EWeaponType::Blaster)
	{
		PXII_LOG(ELogCategory::Ability, Log, TEXT("DZ_LOG:: Showing Blaster"));

		ActiveWeapon = weaponType;
		OwnerCharacter->CurrentWeaponMelee->HideWeapon();
		OwnerCharacter->CurrentWeaponRanged->ShowWeapon();
	}
	OnWeaponActiveUpdated.Broadcast();
}
