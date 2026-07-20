#pragma once
#include "GameplayEffectExecutionCalculation.h"
#include "PxiiDynamicAttributeExecution.generated.h"

UCLASS()
class PXII_API UPxiiDynamicAttributeExecution : public UGameplayEffectExecutionCalculation
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

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
