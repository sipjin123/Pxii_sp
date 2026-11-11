#include "PxiiGameplayEffectYinCost.h"
#include "GECalculations/GEEC_YinCost.h"

UPxiiGameplayEffectYinCost::UPxiiGameplayEffectYinCost()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// Add custom execution calculation
	FGameplayEffectExecutionDefinition ExecutionDefinition;

	ExecutionDefinition.CalculationClass =	UGEEC_YinCost::StaticClass();

	Executions.Add(ExecutionDefinition);
}
