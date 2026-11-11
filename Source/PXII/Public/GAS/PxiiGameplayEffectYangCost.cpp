#include "PxiiGameplayEffectYangCost.h"
#include "GECalculations/GEEC_YangCost.h"

UPxiiGameplayEffectYangCost::UPxiiGameplayEffectYangCost()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// Add custom execution calculation
	FGameplayEffectExecutionDefinition ExecutionDefinition;

	ExecutionDefinition.CalculationClass =	UGEEC_YangCost::StaticClass();

	Executions.Add(ExecutionDefinition);
}
