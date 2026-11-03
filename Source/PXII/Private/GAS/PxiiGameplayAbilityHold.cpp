#include "GAS/PxiiGameplayAbilityHold.h"

#include "Async/ParkingLot.h"
#include "Character/PxiiCharacter.h"

void UPxiiGameplayAbilityHold::ActivateAbility(const FGameplayAbilitySpecHandle Handle,	const FGameplayAbilityActorInfo* OwnerInfo,
                                               const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AActor* Avatar = OwnerInfo->AvatarActor.Get();
	SelfCharacter = Cast<APxiiCharacter>(Avatar);
	HoldTime = GetWorld()->GetTimeSeconds();

	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);
}

void UPxiiGameplayAbilityHold::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (LogData)
	{
		UE_LOG(LogTemp, Warning, TEXT("DZ_LOG:: HOLD END!"));
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UPxiiGameplayAbilityHold::OnHoldReleased_Implementation(float TimeHeld)
{
	HoldTimeFinal = TimeHeld;
	if (LogData)
	{
		UE_LOG(LogTemp, Warning, TEXT("DZ_LOG:: HOLD END Time: %f !"), TimeHeld);
	}
}

void UPxiiGameplayAbilityHold::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	const float HoldDuration = GetWorld()->GetTimeSeconds() - HoldTime;

	OnHoldReleased(HoldDuration);
	if(EndAbilityOnRelease)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}