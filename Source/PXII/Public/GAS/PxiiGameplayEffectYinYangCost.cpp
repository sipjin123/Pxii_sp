#include "PxiiGameplayEffectYinYangCost.h"

#include "PxiiAttributeSet.h"

UPxiiGameplayEffectYinYangCost::UPxiiGameplayEffectYinYangCost()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FSetByCallerFloat yinSetByCaller;
	yinSetByCaller.DataTag = FGameplayTag::RequestGameplayTag("Magnitude.Cost.Yin");

	FSetByCallerFloat yangSetByCaller;
	yangSetByCaller.DataTag = FGameplayTag::RequestGameplayTag("Magnitude.Cost.Yang");

	FGameplayModifierInfo yinMod;
	yinMod.Attribute = UPxiiAttributeSet::GetYinAttribute();
	yinMod.ModifierOp = EGameplayModOp::Additive;
	yinMod.ModifierMagnitude = FGameplayEffectModifierMagnitude(yinSetByCaller);

	FGameplayModifierInfo yangMod;
	yangMod.Attribute = UPxiiAttributeSet::GetYangAttribute();
	yangMod.ModifierOp = EGameplayModOp::Additive;
	yangMod.ModifierMagnitude = FGameplayEffectModifierMagnitude(yangSetByCaller);

	Modifiers.Reset();
	Modifiers.Add(yinMod);
	Modifiers.Add(yangMod);
}
