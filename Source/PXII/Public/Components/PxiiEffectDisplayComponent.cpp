#include "PxiiEffectDisplayComponent.h"

#include "Character/PxiiCharacter.h"
#include "Utility/PXIILogUtility.h"

void UPxiiEffectDisplayComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsValid(GetASC()))
	{
		UE_LOG(LogTemp, Warning, TEXT("GEDisplayText: %s has no AbilitySystemComponent"), *GetNameSafe(GetOwner()));
		return;
	}
 
	AppliedHandle = ASC->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UPxiiEffectDisplayComponent::HandleEffectApplied);
	RemovedHandle = ASC->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &UPxiiEffectDisplayComponent::HandleEffectRemoved);
}

void UPxiiEffectDisplayComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(GetASC()))
	{
		ASC->OnGameplayEffectAppliedDelegateToSelf.Remove(AppliedHandle);
	}
	Super::EndPlay(EndPlayReason);
}

UPxiiAbilitySystemComponent* UPxiiEffectDisplayComponent::GetASC()
{
	if(ASC)
	{
		return ASC;
	}
	
	APxiiCharacter* character = Cast<APxiiCharacter>(GetOwner());
	ASC = Cast<UPxiiAbilitySystemComponent>(character->GetAbilitySystemComponent());

	return ASC;
}

void UPxiiEffectDisplayComponent::HandleEffectApplied(UAbilitySystemComponent* Source, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle)
{
	const UPxiiGameplayEffectBase* GE = Cast<UPxiiGameplayEffectBase>(Spec.Def);
	if (!GE || !GE->EffectDetails.ShowDisplayText || GE->EffectDetails.EffectDisplayText.IsEmpty())
	{
		return;
	}
	
	if (Handle.IsValid()) // instant effects give an invalid handle, so they are never tracked
	{
		// ActiveEffectTexts.Add(Handle, GE->EffectDetails.EffectDisplayText);
		bool isInstant = GE->DurationPolicy == EGameplayEffectDurationType::Instant;
		ShowEffectText(Handle, GE->EffectDetails, isInstant);
	}

	// PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: Effect Text: %s"), *GE->EffectDisplayText.ToString());
}

void UPxiiEffectDisplayComponent::HandleEffectRemoved(const FActiveGameplayEffect& ActiveEffect)
{
	if (!ActiveEffect.Handle.IsValid())
	{
		return;
	}

	PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: REMOVING EFFECT"));

	RemoveEffectText(ActiveEffect.Handle);
}

void UPxiiEffectDisplayComponent::ShowEffectText_Implementation(FActiveGameplayEffectHandle Handle, FEffectDetails GEDetails, bool isInstant)
{
	
}

void UPxiiEffectDisplayComponent::RemoveEffectText_Implementation(FActiveGameplayEffectHandle handle)
{
}
