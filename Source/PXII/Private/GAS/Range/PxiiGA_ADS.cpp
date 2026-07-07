// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Range/PxiiGA_ADS.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Character/PxiiCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerController/PxiiPlayerController.h"

UPxiiGA_ADS::UPxiiGA_ADS(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
}

bool UPxiiGA_ADS::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                     const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	auto* Character = CastChecked<APxiiCharacter>(ActorInfo->AvatarActor.Get());

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UPxiiGA_ADS::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Warning, TEXT("Ability has been Activated!"));
	if (!CommitAbility(Handle, OwnerInfo, ActivationInfo))
	{
		return;
	}

	CurrentPlayerCharacter = CastChecked<APxiiCharacter>(OwnerInfo->AvatarActor.Get());

	// NOTE: As per request, change equipment automatically when aiming
	/*
	if (!CurrentPlayerCharacter->GetIsWeaponEquipped())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}*/

	
	if (!CurrentPlayerCharacter->GetIsADSEnabled())
	{
		CurrentPlayerCharacter->SetIsADSEnabled(true);

		CurrentPlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		CurrentPlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		CurrentPlayerCharacter->bUseControllerRotationYaw = true;

		/*
		// TODO[DHENZ]: Trigger Reticle Logic Here
		APXIIHUD* PXIIHUD = Cast<APXIIHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (PXIIHUD)
		{
			PXIIHUD->SetShowReticle(true);
		}*/

		// TODO[DHENZ]: Trigger Controller Logic Here
		/*
		auto* PlayerController = Cast<APxiiPlayerController>(CurrentPlayerCharacter->GetController());
		PlayerController->SetAimYawScale(0.15f);
		PlayerController->SetAimPitchScale(0.15f);
		
		CurrentPlayerCharacter->AddControllerPitchInput(OffsetVerticalAdjustment);
		CurrentPlayerCharacter->AddControllerYawInput(OffsetHorizontalAdjustment);
		*/
	}
}

void UPxiiGA_ADS::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UE_LOG(LogTemp, Warning, TEXT("Ability has been End!"));
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPxiiGA_ADS::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	UE_LOG(LogTemp, Warning, TEXT("Ability has been Cancel!"));
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}