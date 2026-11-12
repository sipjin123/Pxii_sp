// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PxiiPlayerCombatComponent.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/PxiiTags.h"
#include "Enum/PxiiDamageType.h"
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
	PXII_LOG(ELogCategory::Ability, Log, TEXT("DZ_LOG:: ABILITY ACTIVATED %s"), *ActivatedAbility->GetClass()->GetDisplayNameText().ToString());

	bool isAttackClass = false;
	
	for(auto attackClass : MeleeAttackClasses)
	{
		if(ActivatedAbility->GetClass()->IsChildOf(attackClass))
		{
			isAttackClass = true;
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
	GetWorld()->GetTimerManager().SetTimer(
		LastAttackTimer,
		this,
		&UPxiiPlayerCombatComponent::HideActiveWeapon,
		ShowWeaponTimer,
		false
	);
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
}
