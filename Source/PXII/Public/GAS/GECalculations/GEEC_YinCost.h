#pragma once
#include "GameplayEffectExecutionCalculation.h"
#include "GEEC_YinCost.generated.h"

UCLASS()
class PXII_API UGEEC_YinCost : public UGameplayEffectExecutionCalculation
{
	
	GENERATED_BODY()
	
public:
	
	UGEEC_YinCost();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	
	FGameplayEffectAttributeCaptureDefinition YinCaptureDef;
	FGameplayEffectAttributeCaptureDefinition YangCaptureDef;
	FGameplayEffectAttributeCaptureDefinition MaxYinCaptureDef;
	FGameplayEffectAttributeCaptureDefinition MaxYangCaptureDef;	
};
