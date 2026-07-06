#include "Input/PxiiAbilityRouterManager.h"

void UPxiiAbilityRouterManager::InitializeASC(UAbilitySystemComponent* InASC)
{
	AbilitySystemComponent = Cast<UPxiiAbilitySystemComponent>(InASC);
}

bool UPxiiAbilityRouterManager::HandleInputPressed(const FGameplayTag& InputTag)
{
	if (!AbilitySystemComponent)
	{
		return false;
	}

	return AbilitySystemComponent->AbilityInputTagPressed(InputTag);
}

void UPxiiAbilityRouterManager::HandleInputReleased(const FGameplayTag& InputTag)
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	AbilitySystemComponent->AbilityInputTagReleased(InputTag);
}
