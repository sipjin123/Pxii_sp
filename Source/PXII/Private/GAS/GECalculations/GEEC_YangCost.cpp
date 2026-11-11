#include "GAS/GECalculations/GEEC_YangCost.h"
#include "GAS/PxiiAttributeSet.h"

UGEEC_YangCost::UGEEC_YangCost()
{
	YinCaptureDef =	FGameplayEffectAttributeCaptureDefinition(
		UPxiiAttributeSet::GetYinAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false);

	YangCaptureDef = FGameplayEffectAttributeCaptureDefinition(
		UPxiiAttributeSet::GetYangAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false);

	MaxYangCaptureDef = FGameplayEffectAttributeCaptureDefinition(
	UPxiiAttributeSet::GetMaxYangAttribute(),
	EGameplayEffectAttributeCaptureSource::Target,
	false);
	
	MaxYinCaptureDef = FGameplayEffectAttributeCaptureDefinition(
		UPxiiAttributeSet::GetMaxYinAttribute(),
		EGameplayEffectAttributeCaptureSource::Target,
		false);
	
	RelevantAttributesToCapture.Add(YinCaptureDef);
	RelevantAttributesToCapture.Add(YangCaptureDef);

	RelevantAttributesToCapture.Add(MaxYangCaptureDef);
	RelevantAttributesToCapture.Add(MaxYinCaptureDef);
}

void UGEEC_YangCost::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Get Attribute A
	float YinAttrib= 0.0f;
	float YangAttrib= 0.0f;
	float MaxYinAttrib= 0.0f;
	float MaxYangAttrib= 0.0f;

	FAggregatorEvaluateParameters EvaluationParameters;
	const bool yinCaptured = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(YinCaptureDef, EvaluationParameters,YinAttrib);
	const bool yangCaptured = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(YangCaptureDef, EvaluationParameters,YangAttrib);
	const bool maxYinCaptured = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MaxYinCaptureDef, EvaluationParameters,MaxYinAttrib);
	const bool maxYangCaptured = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MaxYangCaptureDef, EvaluationParameters,MaxYangAttrib);
	
	if (!yinCaptured || !yangCaptured || !maxYinCaptured || !maxYangCaptured)
	{
		return;
	}

	const FGameplayTag AmountTag = FGameplayTag::RequestGameplayTag(FName("Magnitude.Cost"));
	const float Cost = Spec.GetSetByCallerMagnitude(AmountTag, false, 0.0f);

	// Prevent negative input
	float Amount = FMath::Max(0.0f, Cost);
	const float Reduction = FMath::Min(YangAttrib, Amount);
	
	if (Reduction > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				UPxiiAttributeSet::GetYangAttribute(),
				EGameplayModOp::Additive,
				-Reduction
			)
		);
	}
	const float YinCapacity = FMath::Max(0.0f, MaxYinAttrib - YinAttrib);
	const float YinIncrease = FMath::Min(Reduction, YinCapacity);
	
	if(YinIncrease > 0.0f)
	{
		// B += X
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				UPxiiAttributeSet::GetYinAttribute(),
				EGameplayModOp::Additive,
				YinIncrease
			)
		);
	}

	// OLD Implementation
	// const float Excess = Amount - Reduction;

	// if (YangAttrib > 0.0f)
	// {
	// 	OutExecutionOutput.AddOutputModifier(
	// 		FGameplayModifierEvaluatedData(
	// 			UPxiiAttributeSet::GetYangAttribute(),
	// 			EGameplayModOp::Additive,
	// 			-Reduction
	// 		)
	// 	);
	// }
	// const float YinCapacity = FMath::Max(0.0f, MaxYinAttrib - YinAttrib);
	// const float YinIncrease = FMath::Min(Excess, YinCapacity);
	//
	// if(YinIncrease > 0.0f)
	// {
	// 	// B += X
	// 	OutExecutionOutput.AddOutputModifier(
	// 		FGameplayModifierEvaluatedData(
	// 			UPxiiAttributeSet::GetYinAttribute(),
	// 			EGameplayModOp::Additive,
	// 			YinIncrease
	// 		)
	// 	);
	// }
}
