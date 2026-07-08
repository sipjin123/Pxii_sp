#pragma once
#include "PxiiGameplayAbilityBase.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "PxiiGameplayAbilityHold.generated.h"
UCLASS()
class PXII_API UPxiiGameplayAbilityHold : public UPxiiGameplayAbilityBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	bool EndAbilityOnRelease = true;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;	

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnHoldReleased(float TimeHeld);
	UPROPERTY(BlueprintReadWrite)
	float HoldTimeFinal;

private:

	float HoldTime;
};


