#include "PxiiYinYangComponent.h"
#include "GameplayEffect.h"
#include "Character/PxiiCharacter.h"
#include "GAS/PxiiAbilitySystemComponent.h"
#include "GAS/PxiiGameplayEffectYangCost.h"
#include "GAS/PxiiGameplayEffectYinCost.h"
#include "GAS/PxiiGameplayEffectYinYangCost.h"
#include "Utility/PXIILogUtility.h"

void UPxiiYinYangComponent::ApplyArmbladeCost_Implementation()
{
	
}

void UPxiiYinYangComponent::ApplyCost_Implementation(EWeaponResourceType WeaponType, float cost)
{
	if (!IsValid(GetASC()))
	{
		PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: INVALID ASC"));
		return;
	}

	// Create Gameplay Effect Context
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	// Create Gameplay Effect Spec
	FGameplayEffectSpecHandle SpecHandle = GetCostEffect(WeaponType, EffectContext);

	if (!SpecHandle.IsValid())
	{
		PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: INVALID SPEC"));
		return;
	}

	// Set dynamic cost
	const FGameplayTag AmountTag =	FGameplayTag::RequestGameplayTag(FName("Magnitude.Cost"));
	SpecHandle.Data->SetSetByCallerMagnitude(AmountTag,	cost);

	// Apply to self
	const FActiveGameplayEffectHandle AppliedHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UPxiiYinYangComponent::ApplyYinYangStressEffect_Implementation(EWeaponResourceType WeaponType)
{
	if (!IsValid(GetASC()))
	{
		PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: INVALID ASC"));
		return;
	}

	// Create Gameplay Effect Context
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	// Create Gameplay Effect Spec
	FGameplayEffectSpecHandle SpecHandle = GetStressEffect(WeaponType, EffectContext);

	if (!SpecHandle.IsValid())
	{
		PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: INVALID Stress SPEC"));
		return;
	}

	// Apply to self
	StressGEHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UPxiiYinYangComponent::DisableYinYangStressEffect_Implementation(EWeaponResourceType WeaponType)
{
	if (StressGEHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(StressGEHandle);
		StressGEHandle.Invalidate();
	}
}

void UPxiiYinYangComponent::ApplyYinYangOverwhelmEffect_Implementation(EWeaponResourceType WeaponType)
{
	if (!IsValid(GetASC()))
	{
		PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: INVALID ASC"));
		return;
	}

	// Create Gameplay Effect Context
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	// Create Gameplay Effect Spec
	FGameplayEffectSpecHandle SpecHandle = GetOverwhelmEffect(WeaponType, EffectContext);

	if (!SpecHandle.IsValid())
	{
		PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: INVALID SPEC"));
		return;
	}

	// Apply to self
	OverwhelmGEHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UPxiiYinYangComponent::DisableYinYangOverwhelmEffect_Implementation(EWeaponResourceType WeaponType)
{
	if (OverwhelmGEHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(OverwhelmGEHandle);
		OverwhelmGEHandle.Invalidate();
	}
}

void UPxiiYinYangComponent::ApplyYinYangFullEffect_Implementation(EWeaponResourceType WeaponType)
{
	if (!IsValid(GetASC()))
	{
		PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: INVALID ASC"));
		return;
	}

	// Create Gameplay Effect Context
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	// Create Gameplay Effect Spec
	FGameplayEffectSpecHandle SpecHandle = GetFullEffect(WeaponType, EffectContext);

	if (!SpecHandle.IsValid())
	{
		PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: INVALID SPEC"));
		return;
	}

	// Apply to self
	FullGEHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UPxiiYinYangComponent::DisableYinYangFullEffect_Implementation(EWeaponResourceType WeaponType)
{
	if (FullGEHandle.IsValid())
	{
		ASC->RemoveActiveGameplayEffect(FullGEHandle);
		FullGEHandle.Invalidate();
	}
}

FGameplayEffectSpecHandle UPxiiYinYangComponent::GetCostEffect(EWeaponResourceType WeaponType, FGameplayEffectContextHandle context)
{
	if(WeaponType == EWeaponResourceType::Yang)
	{
		return ASC->MakeOutgoingSpec(UPxiiGameplayEffectYinCost::StaticClass(), 1.0f, context);
	}

	return ASC->MakeOutgoingSpec(UPxiiGameplayEffectYangCost::StaticClass(), 1.0f, context);
}

FGameplayEffectSpecHandle UPxiiYinYangComponent::GetStressEffect(EWeaponResourceType WeaponType,
	FGameplayEffectContextHandle context)
{
	if(YinYangData == nullptr)
	{
		return nullptr;
	}

	if(WeaponType == EWeaponResourceType::Yang)
	{
		return ASC->MakeOutgoingSpec(YinYangData->YangStressGE, 1.0f, context);
	}
	
	return ASC->MakeOutgoingSpec(YinYangData->YinStressGE, 1.0f, context);
}

FGameplayEffectSpecHandle UPxiiYinYangComponent::GetOverwhelmEffect(EWeaponResourceType WeaponType,
	FGameplayEffectContextHandle context)
{
	if(YinYangData == nullptr)
	{
		return nullptr;
	}
	
	if(WeaponType == EWeaponResourceType::Yang)
	{
		return ASC->MakeOutgoingSpec(YinYangData->YangOverwhelmGE, 1.0f, context);
	}

	return ASC->MakeOutgoingSpec(YinYangData->YinOverwhelmGE, 1.0f, context);
}

FGameplayEffectSpecHandle UPxiiYinYangComponent::GetFullEffect(EWeaponResourceType WeaponType,
	FGameplayEffectContextHandle context)
{
	if(YinYangData == nullptr)
	{
		return nullptr;
	}
	
	if(WeaponType == EWeaponResourceType::Yang)
	{
		return ASC->MakeOutgoingSpec(YinYangData->YangFullGE, 1.0f, context);
	}

	return ASC->MakeOutgoingSpec(YinYangData->YinFullGE, 1.0f, context);
}

UPxiiYinYangData* UPxiiYinYangComponent::GetYinYangData()
{
	return YinYangData;
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
