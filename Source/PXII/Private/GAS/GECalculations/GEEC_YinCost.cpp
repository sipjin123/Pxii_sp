#include "GAS/GECalculations/GEEC_YinCost.h"
#include "GAS/PxiiAttributeSet.h"
#include "Utility/PXIILogUtility.h"

UGEEC_YinCost::UGEEC_YinCost()
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

void UGEEC_YinCost::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
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
		PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: FAILED RETURN"));
		return;
	}

	const FGameplayTag AmountTag = FGameplayTag::RequestGameplayTag(FName("Magnitude.Cost"));
	const float Cost = Spec.GetSetByCallerMagnitude(AmountTag, false, 0.0f);

	// Prevent negative input
	const float Amount = FMath::Max(0.0f, Cost);
	const float Reduction = FMath::Min(YinAttrib, Amount);

	if (Reduction > 0.0f)
	{
		// PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: REDUCTION %f"), -Reduction);

		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				UPxiiAttributeSet::GetYinAttribute(),
				EGameplayModOp::Additive,
				-Reduction
			)
		);
	}

	const float YangCapacity = FMath::Max(0.0f, MaxYangAttrib - YangAttrib);
	const float YangIncrease = FMath::Min(Reduction, YangCapacity);
	
	if(YangIncrease > 0.0f)
	{
		// PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: INCREASE %f"), YangIncrease);

		// B += X
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				UPxiiAttributeSet::GetYangAttribute(),
				EGameplayModOp::Additive,
				YangIncrease
			)
		);
	}

	// OLD Implementation
	// const float Excess = Amount - Reduction;
	//
	// if (YinAttrib > 0.0f)
	// {
	// 	OutExecutionOutput.AddOutputModifier(
	// 		FGameplayModifierEvaluatedData(
	// 			UPxiiAttributeSet::GetYinAttribute(),
	// 			EGameplayModOp::Additive,
	// 			-Reduction
	// 		)
	// 	);
	// }
	//
	// const float YangCapacity = FMath::Max(0.0f, MaxYangAttrib - YangAttrib);
	// const float YangIncrease = FMath::Min(Excess, YangCapacity);
	//
	// if(YangIncrease > 0.0f)
	// {
	// 	// B += X
	// 	OutExecutionOutput.AddOutputModifier(
	// 		FGameplayModifierEvaluatedData(
	// 			UPxiiAttributeSet::GetYangAttribute(),
	// 			EGameplayModOp::Additive,
	// 			YangIncrease
	// 		)
	// 	);
	// }
}
