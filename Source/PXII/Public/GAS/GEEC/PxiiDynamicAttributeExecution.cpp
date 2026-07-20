#include "PxiiDynamicAttributeExecution.h"
#include "Utility/PXIILogUtility.h"

UPxiiDynamicAttributeExecution::UPxiiDynamicAttributeExecution()
{
	RelevantAttributesToCapture.Add(
	FGameplayEffectAttributeCaptureDefinition(
		BaseValueAttribute,
		EGameplayEffectAttributeCaptureSource::Target,
		false));

	RelevantAttributesToCapture.Add(
		FGameplayEffectAttributeCaptureDefinition(
			FlatBonusAttribute,
			EGameplayEffectAttributeCaptureSource::Target,
			false));

	RelevantAttributesToCapture.Add(
		FGameplayEffectAttributeCaptureDefinition(
			ModifierAttribute,
			EGameplayEffectAttributeCaptureSource::Target,
			false));
}

void UPxiiDynamicAttributeExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                            FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	PXII_LOG(ELogCategory::General, Log, TEXT("DZ_LOG:: GEEC EXECUTION"));

	FGameplayEffectAttributeCaptureDefinition BaseCapture(
		BaseValueAttribute,
		EGameplayEffectAttributeCaptureSource::Target,
		false);

	FGameplayEffectAttributeCaptureDefinition BonusCapture(
		FlatBonusAttribute,
		EGameplayEffectAttributeCaptureSource::Target,
		false);

	FGameplayEffectAttributeCaptureDefinition ModifierCapture(
		ModifierAttribute,
		EGameplayEffectAttributeCaptureSource::Target,
		false);
	
	float BaseValue = 0.0f;
	float FlatBonusValue = 0.0f;
	float ModifierBonus = 1.0f;
	float TestVal = 0.f;
	FAggregatorEvaluateParameters Params;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		RelevantAttributesToCapture[0],
		Params,
		TestVal);
	PXII_LOG(ELogCategory::General, Log, TEXT("DZ_LOG:: GEEC TestVal : %f"), TestVal);
	
	if(BaseCapture.AttributeToCapture.IsValid())
	{
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BaseCapture, Params,BaseValue);
	}

	if(BonusCapture.AttributeToCapture.IsValid())
	{
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BonusCapture, Params,FlatBonusValue);
	}
	
	if(ModifierCapture.AttributeToCapture.IsValid())
	{
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ModifierCapture, Params,ModifierBonus);
	}

	float FinalValue = (BaseValue + FlatBonusValue) * ModifierBonus;
	PXII_LOG(ELogCategory::General, Log, TEXT("DZ_LOG:: GEEC FinalValue : %f"), FinalValue);
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(OutputAttribute, EGameplayModOp::Override, FinalValue));
}
