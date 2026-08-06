// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GECalculations/GEEC_DynamicStagger.h"
#include "AbilitySystemComponent.h"
#include "Character/PxiiNPC.h"
#include "GAS/PxiiAttributeSet.h"
#include "Interface/PxiiCombatInterface.h"

DEFINE_LOG_CATEGORY(LogGEECStagger);
struct StaggerStatCapture
{
	// Declares the relevant variable that will be captured from attribute of a target
	DECLARE_ATTRIBUTE_CAPTUREDEF(StaggerMeter);

	StaggerStatCapture()
	{
		// Defines the relevant variable that will be captured from attribute of a target
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPxiiAttributeSet, StaggerMeter, Target, false);
	}
};

static StaggerStatCapture& GetStaggerStatCapture()
{
	static StaggerStatCapture StatCapture;
	return StatCapture;
}

UGEEC_DynamicStagger::UGEEC_DynamicStagger()
{
}

void UGEEC_DynamicStagger::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                 FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	AActor* SourceActor = Cast<AActor>(ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor());
	AActor* TargetActor = Cast<AActor>(ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor());
	
	// Handle Tags
	const FGameplayEffectSpec& GESpec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = GESpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = GESpec.CapturedTargetTags.GetAggregatedTags();

	// Generate Parameters using tags fetched
	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTags;
	EvalParams.TargetTags = TargetTags;

	float CurrentHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetStaggerStatCapture().StaggerMeterDef, EvalParams, CurrentHealth);

	const float TargetStaggerMeter = GESpec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Combat.Damage.Stagger")), // Must match GE tag
		false, 0.0f);

	auto CurrMeter = Cast<APxiiNPC>(TargetActor)->GetAttributeSet()->GetStaggerMeter();
	//UE_LOG(LogGEECStagger, Warning, TEXT("GEEC Stagger Dmg :: Name: {%s} ToApply: {%f} Curr: {%f}"), *TargetActor->GetName(), TargetStaggerMeter, CurrMeter);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetStaggerStatCapture().StaggerMeterProperty, EGameplayModOp::Additive, TargetStaggerMeter));
}
