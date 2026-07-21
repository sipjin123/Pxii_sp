#include "PxiiDynamicAttributeExecution.h"
#include "AbilitySystemComponent.h"
#include "Utility/PXIILogUtility.h"

UPxiiDynamicAttributeExecution::UPxiiDynamicAttributeExecution()
{
	
}

float UPxiiDynamicAttributeExecution::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	PXII_LOG(ELogCategory::General, Log, TEXT("DZ_LOG:: GEEC EXECUTION"));

	UAbilitySystemComponent* SourceASC = Spec.GetContext().GetOriginalInstigatorAbilitySystemComponent();

	float BaseValue = 0.0f;
	float Bonus = 0.0f;
	float Modifier = 1.0f;
	
	BaseValue = SourceASC->GetNumericAttribute(BaseValueAttribute);
	Bonus = SourceASC->GetNumericAttribute(FlatBonusAttribute);
	Modifier = SourceASC->GetNumericAttribute(ModifierAttribute);

	float Result = (BaseValue + Bonus) * Modifier;
	
	PXII_LOG(ELogCategory::General, Log, TEXT("DZ_LOG:: GEEC Result : %f"), Result);

	return Result;
}
