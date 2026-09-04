// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GECalculations/GEEC_DynamicDamage.h"

#include "AbilitySystemComponent.h"
#include "Engine/World.h"
#include "Enum/PxiiDamageType.h"
#include "GAS/PxiiAttributeSet.h"
#include "Interface/PxiiCombatInterface.h"
#include "Subsystem/PxiiCombatRegistrySubsystem.h"
#include "Subsystem/WorldSpawnerSubsystem.h"

DEFINE_LOG_CATEGORY(LogGEECDamage);
struct CombatStatCapture
{
	// Declares the relevant variable that will be captured from attribute of a target
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	CombatStatCapture()
	{
		// Defines the relevant variable that will be captured from attribute of a target
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPxiiAttributeSet, Health, Target, false);
		//sDEFINE_ATTRIBUTE_CAPTUREDEF(UPxiiAttributeSet, DamageReceived, Target, false);
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UPxiiAttributeSet, WasCriticalHit, Target, false);
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UPxiiAttributeSet, RagePoints, Target, false);
	}
};

static CombatStatCapture& GetCombatStatCapture()
{
	static CombatStatCapture StatCapture;
	return StatCapture;
}

UGEEC_DynamicDamage::UGEEC_DynamicDamage()
{
}

void UGEEC_DynamicDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
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

	//UE_LOG(LogGEECDamage, Warning, TEXT("GEEC Start"));
	// Capture relevant attributes
	float CurrentHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetCombatStatCapture().HealthDef, EvalParams, CurrentHealth);

	bool bIsCritical = false;
	float IncomingDamage = GESpec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Combat.Damage")), // Must match GE tag
		false, 0.0f);

	float DamageSource = GESpec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Combat.DamageSource")), // Must match GE tag
		false,
		0.0f // Default value if not found
	);

	const float HitTypeValue = GESpec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Combat.HitType")),
		false,
		0.0f
	);

	const EHitEffectType HitEffectType = static_cast<EHitEffectType>(static_cast<uint8>(HitTypeValue));
	
	bool BlockDamageProcessing = false;
	static const FGameplayTag BlockingTag = FGameplayTag::RequestGameplayTag(TEXT("Combat.State.Blocking"));
	static const FGameplayTag PerfectDodgeTag = FGameplayTag::RequestGameplayTag(TEXT("Combat.State.PerfectDodgeWindow"));
	static const FGameplayTag ParryTag = FGameplayTag::RequestGameplayTag(TEXT("Combat.State.ParryWindow"));

	const bool bIsTargetBlocking = ExecutionParams.GetTargetAbilitySystemComponent()->HasMatchingGameplayTag(BlockingTag);
	const bool bIsTargetParry = ExecutionParams.GetTargetAbilitySystemComponent()->HasMatchingGameplayTag(ParryTag);
	const bool bIsTargetDodge = ExecutionParams.GetTargetAbilitySystemComponent()->HasMatchingGameplayTag(PerfectDodgeTag);
	
	bool isCritical = false;
	//UE_LOG(LogGEECDamage, Warning, TEXT("------------------ STATE: %d %d %d"), !bIsTargetBlocking ? 0 : 1, !bIsTargetParry ? 0 : 1,  !bIsTargetDodge ? 0 : 1);

	if (bIsTargetBlocking)
		IncomingDamage *= .5f;
	if (bIsTargetParry)
	{
		BlockDamageProcessing = true;
		IncomingDamage = 0;
	}
	if (bIsTargetDodge)
	{
		BlockDamageProcessing = true;
		IncomingDamage = 0;
	}
	
	//-------------------------------------------------------------
	if (SourceActor && SourceActor->Implements<UPxiiCombatInterface>())
	{
		IPxiiCombatInterface::Execute_ProcessDamageData(SourceActor, SourceActor, IncomingDamage, DamageSource);
		
		if (bIsTargetParry)
		IPxiiCombatInterface::Execute_TriggerSpecialAction(TargetActor, ESpecialAction::Parry, 0, SourceActor);

		if (bIsTargetDodge)
			IPxiiCombatInterface::Execute_TriggerSpecialAction(TargetActor, ESpecialAction::PerfectDodge, 0, SourceActor);
		if (BlockDamageProcessing)
		{
			return;
		}

		// TODO: Clean and optimize damage notifs
		FDamageNotifPayload AttackerPayload;
		AttackerPayload.bIsCritical = isCritical;
		AttackerPayload.DamageMagnitude = IncomingDamage;
		AttackerPayload.TargetActor = TargetActor;
		AttackerPayload.HitType = HitEffectType;
		AttackerPayload.UTCTime = SourceActor->GetWorld()->GetTimeSeconds();
		
		IPxiiCombatInterface::Execute_NotifyHitTarget(SourceActor, TargetActor, AttackerPayload);
		
		FDamageNotifPayload DefenderPayload;
		DefenderPayload.bIsCritical = isCritical;
		DefenderPayload.DamageMagnitude = IncomingDamage;
		DefenderPayload.TargetActor = TargetActor;
		DefenderPayload.HitType = HitEffectType;
		DefenderPayload.UTCTime = SourceActor->GetWorld()->GetTimeSeconds();
		IPxiiCombatInterface::Execute_NotifyHasReceivedDamage(TargetActor, SourceActor, DefenderPayload);
	}
	//OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCombatStatCapture().WasCriticalHitProperty, EGameplayModOp::Override, bIsCritical ? 1.0 : 0.0));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetCombatStatCapture().HealthProperty, EGameplayModOp::Additive, -IncomingDamage));

	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	if (TargetASC)
	{
		if (UWorld* World = TargetASC->GetWorld())
		{
			if (UWorldSpawnerSubsystem* Spawner = World->GetSubsystem<UWorldSpawnerSubsystem>())
			{
				const FGameplayEffectContextHandle& Context = GESpec.GetContext();

				if (Context.IsValid())
				{
					const FVector HitLocation = Context.GetOrigin();
					//UE_LOG(LogGEECDamage, Warning, TEXT("Dmg Number is: %s"), *TargetActor->GetName());
					Spawner->OnSpawnDamageText.Broadcast(HitLocation, IncomingDamage, bIsCritical);
				}
			}
			if (UPxiiCombatRegistrySubsystem* CombatSubsystem = World->GetSubsystem<UPxiiCombatRegistrySubsystem>())
			{
				TSubclassOf<UGameplayEffect> GEECStagger = CombatSubsystem->GetGEECStagger();
				if (GEECStagger && TargetASC)
				{
					FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
					EffectContext.AddSourceObject(this);
					FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(GEECStagger, 1.f, EffectContext);

					if (SpecHandle.IsValid())
					{
						// Override the Gameplay Effect duration.
						SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Combat.Damage.Stagger")), 10.f);
						TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
					}
				}
			}
		}
	}
	if (bIsCritical)
	{
		//ExecutionParams.GetOwningSpecForPreExecuteMod()->DynamicGrantedTags.AddTag(TAG_Damage_Crit);
	}
}
