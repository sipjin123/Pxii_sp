#pragma once
#include "GAS/PxiiGameplayAbilityBase.h"
#include "Character/PxiiCharacter.h"
#include "HUD/PxiiHUDBase.h"
#include "PxiiGA_AdsToggle.generated.h"

UCLASS(Blueprintable)
class UPxiiGA_AdsToggle : public UPxiiGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	explicit UPxiiGA_AdsToggle(const FObjectInitializer& ObjectInitializer);

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
 
private:

	UPROPERTY()
	TObjectPtr<APxiiCharacter> CurrentPlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool LogInfo = false;
};
