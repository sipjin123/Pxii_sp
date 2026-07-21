#pragma once
#include "GameplayModMagnitudeCalculation.h"
#include "GameplayEffectExecutionCalculation.h"
#include "PxiiDynamicAttributeExecution.generated.h"

UCLASS()
class PXII_API UPxiiDynamicAttributeExecution : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

	UPxiiDynamicAttributeExecution();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FGameplayAttribute BaseValueAttribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FGameplayAttribute FlatBonusAttribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FGameplayAttribute ModifierAttribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Calculation")
	FGameplayAttribute OutputAttribute;

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

};
