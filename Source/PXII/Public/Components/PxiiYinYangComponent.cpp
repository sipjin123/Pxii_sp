#include "PxiiYinYangComponent.h"
#include "GameplayEffect.h"
#include "Character/PxiiCharacter.h"
#include "GAS/PxiiAbilitySystemComponent.h"
#include "GAS/PxiiGameplayEffectYinYangCost.h"

void UPxiiYinYangComponent::ApplyCost_Implementation(UResourceGenerationCost* cost)
{
	if (!GetASC())
	{
		return;
	}
	
	if(!cost || !cost->IsValid())
	{
		return;
	}
	
	TSubclassOf<UGameplayEffect> CostGameplayEffect = UPxiiGameplayEffectYinYangCost::StaticClass();
	if(CostGameplayEffect)
	{
		FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CostGameplayEffect, 1.0f, Context);
		if(!SpecHandle.IsValid())
		{
			return;
		}

		float yinMod = GetFinalCost(EWeaponResourceType::Yin, cost);
		float yangMod = GetFinalCost(EWeaponResourceType::Yang, cost);

		SpecHandle.Data->SetSetByCallerMagnitude(YinTag, yinMod);
		SpecHandle.Data->SetSetByCallerMagnitude(YangTag, yangMod);

		GetASC()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

float UPxiiYinYangComponent::GetFinalCost(EWeaponResourceType resourceType,
	UResourceGenerationCost* costData) const
{
	if(costData->CostType == resourceType)
	{
		return -costData->CostValue;
	}

	if(costData->GenerateType == resourceType)
	{
		return costData->GenerateValue;
	}

	return 0.0f;
}

UPxiiAbilitySystemComponent* UPxiiYinYangComponent::GetASC()
{
	if(ASC)
	{
		return ASC;
	}

	
	APxiiCharacter* character = Cast<APxiiCharacter>(GetOwner());
	ASC = Cast<UPxiiAbilitySystemComponent>(character->GetAbilitySystemComponent());

	return ASC;
}
