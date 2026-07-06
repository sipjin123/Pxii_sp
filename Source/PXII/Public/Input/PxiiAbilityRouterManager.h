#pragma once
#include "PlayerInputManagerBase.h"
#include "GAS/PxiiAbilitySystemComponent.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "PxiiAbilityRouterManager.generated.h"

UCLASS()
class PXII_API UPxiiAbilityRouterManager : public UPlayerInputManagerBase
{
	GENERATED_BODY()
	
public:

	void InitializeASC(UAbilitySystemComponent* InASC);

	bool HandleInputPressed(const FGameplayTag& InputTag);

	void HandleInputReleased(const FGameplayTag& InputTag);
	
private:

	UPROPERTY()
	TObjectPtr<UPxiiAbilitySystemComponent> AbilitySystemComponent;
};
