#pragma once
#include "GameplayEffectAttributeCaptureDefinition.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_YangCost.generated.h"

UCLASS()
class PXII_API UGEEC_YangCost : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	
	UGEEC_YangCost();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	
	FGameplayEffectAttributeCaptureDefinition YinCaptureDef;
	FGameplayEffectAttributeCaptureDefinition YangCaptureDef;
	FGameplayEffectAttributeCaptureDefinition MaxYinCaptureDef;
	FGameplayEffectAttributeCaptureDefinition MaxYangCaptureDef;	
	
};
