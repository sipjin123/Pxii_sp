#include "GAS/Range/PxiiGA_AdsToggle.h"


UPxiiGA_AdsToggle::UPxiiGA_AdsToggle(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
}

bool UPxiiGA_AdsToggle::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UPxiiGA_AdsToggle::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(LogInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability has been Activated!"));
	}
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		if(LogInfo)
		{
			UE_LOG(LogTemp, Warning, TEXT("Ability has been Activated!"));
		}
		
		return;
	}


	CurrentPlayerCharacter = CastChecked<APxiiCharacter>(ActorInfo->AvatarActor.Get());
	APxiiHUDBase* PXIIHUD = Cast<APxiiHUDBase>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!CurrentPlayerCharacter->GetIsADSEnabled())
	{
		if(LogInfo)
		{
			UE_LOG(LogTemp, Warning, TEXT("DZ_LOG:: GetIsADSEnabled"));
		}

		CurrentPlayerCharacter->SetIsADSEnabled(true);

		if (PXIIHUD)
		{
			if(PXIIHUD->HUDLayout)
			{
				PXIIHUD->HUDLayout->ToggleADS(true);
			}
		}
	}
	else
	{
		CurrentPlayerCharacter->SetIsADSEnabled(false);
		if (PXIIHUD)
		{
			if(PXIIHUD->HUDLayout)
			{
				PXIIHUD->HUDLayout->ToggleADS(false);
			}
		}
	}
}

void UPxiiGA_AdsToggle::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if(LogInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability has been End!"));
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPxiiGA_AdsToggle::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	if(LogInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability has been Cancel!"));
	}
	
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}
