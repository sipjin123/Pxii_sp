// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GECalculations/GEEC_DynamicYinYang.h"

#include "AbilitySystemComponent.h"
#include "Components/PxiiPlayerCombatComponent.h"
#include "Data/PxiiTags.h"
#include "GAS/PxiiAttributeSet.h"

DEFINE_LOG_CATEGORY(LogGEECYinYang);
struct YinYangCapture
{
	// Declares the relevant variable that will be captured from attribute of a target
	DECLARE_ATTRIBUTE_CAPTUREDEF(Yang);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Yin);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	YinYangCapture()
	{
		// Defines the relevant variable that will be captured from attribute of a target
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPxiiAttributeSet, Yang, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPxiiAttributeSet, Yin, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPxiiAttributeSet, Health, Source, false);
	}
};

static YinYangCapture& GetYinYangCapture()
{
	static YinYangCapture StatCapture;
	return StatCapture;
}

UGEEC_DynamicYinYang::UGEEC_DynamicYinYang()
{
    UE_LOG(LogTemp, Warning, TEXT("GEEC Constructor"));
	RelevantAttributesToCapture.Add(GetYinYangCapture().HealthDef);
	RelevantAttributesToCapture.Add(GetYinYangCapture().YinDef);
	RelevantAttributesToCapture.Add(GetYinYangCapture().YangDef);
}

void UGEEC_DynamicYinYang::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	// Handle Tags
	const FGameplayEffectSpec& GESpec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = GESpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = GESpec.CapturedTargetTags.GetAggregatedTags();

	// Generate Parameters using tags fetched
	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTags;
	EvalParams.TargetTags = TargetTags;

	UE_LOG(LogGEECYinYang, Warning, TEXT("GEEC Start"));
	// Capture relevant attributes
	float CurrentYin = 0.0f;
	float CurrentYang = 0.0f;
	float CurrentHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetYinYangCapture().YangDef, EvalParams, CurrentYang);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetYinYangCapture().YinDef, EvalParams, CurrentYin);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetYinYangCapture().HealthDef, EvalParams, CurrentHealth);

	const float Type      = GESpec.GetSetByCallerMagnitude(TAG_Data_YinYangType);
	const float Amount    = GESpec.GetSetByCallerMagnitude(TAG_Data_Amount);
	const bool bPercent   = GESpec.GetSetByCallerMagnitude(TAG_Data_IsPercent) > 0.5f;
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UPxiiAttributeSet* AttrSet = SourceASC->GetSet<UPxiiAttributeSet>();

	UE_LOG(LogTemp, Warning, TEXT("Direct: Type=%f AMT=%f Percent=%f"), Type, Amount, bPercent ? 1.0f : 0.f);
	UE_LOG(LogTemp, Warning, TEXT("Direct: Health=%f Yin=%f Yang=%f"), AttrSet->GetHealth(), AttrSet->GetYin(), AttrSet->GetYang());


	bool UseTagsFromBP = false;
	if (UseTagsFromBP)
	{
		float IncomingDamage = GESpec.GetSetByCallerMagnitude(
			FGameplayTag::RequestGameplayTag(FName("Combat.Damage")), // Must match GE tag
			false,
			0.0f // Default value if not found
		);

		float DamageSource = GESpec.GetSetByCallerMagnitude(
			FGameplayTag::RequestGameplayTag(FName("Combat.DamageSource")), // Must match GE tag
			false,
			0.0f // Default value if not found
		);
		
		bool bYin = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetYinYangCapture().YinDef, EvalParams, CurrentYin);
		UE_LOG(LogTemp, Warning, TEXT("Capture Attribute = %s"), *GetYinYangCapture().YinDef.AttributeToCapture.GetName());
		UE_LOG(LogTemp, Warning, TEXT("bYin=%d Value=%f"), bYin, CurrentYin);
		
		UE_LOG(LogGEECYinYang, Warning, TEXT("Yin Value is: %f Yang is: %f :: HP: %f Dmg: %f Source: %f"), CurrentYin, CurrentYang, CurrentHealth, IncomingDamage, DamageSource);
		//float IncomingDamage = 5.f;
		//-------------------------------------------------------------
		IncomingDamage = 10;
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetYinYangCapture().YangProperty, EGameplayModOp::Additive, IncomingDamage));
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetYinYangCapture().YinProperty, EGameplayModOp::Additive, IncomingDamage));
	}
	else
	{
		EPxiiYinYangType YinYangType = static_cast<EPxiiYinYangType>(FMath::RoundToInt(Type));
		if (YinYangType == EPxiiYinYangType::Yin)
		{
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetYinYangCapture().YinProperty, EGameplayModOp::Additive, Amount));
		}
		else
		{
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetYinYangCapture().YangProperty, EGameplayModOp::Additive, Amount));
		}
	}
}
