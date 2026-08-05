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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FireProjectile(APxiiCharacter* character);
	
	void FireProjectile_Implementation(APxiiCharacter* Character);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RequestProjectile(const FVector& projectilDir);

	void RequestProjectile_Implementation(const FVector& projectilDir);	
	
private:
	
	FName MuzzleSocketName;
};
