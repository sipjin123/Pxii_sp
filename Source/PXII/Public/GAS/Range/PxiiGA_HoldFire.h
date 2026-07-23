#pragma once
#include "Character/PxiiCharacter.h"
#include "GAS/PxiiGameplayAbilityHold.h"
#include "PxiiGA_HoldFire.generated.h"

UCLASS()
class PXII_API UPxiiGA_HoldFire : public UPxiiGameplayAbilityHold
{
	GENERATED_BODY()
	
public:
	UPxiiGA_HoldFire();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	virtual void FireProjectile(APxiiCharacter* character);
};
