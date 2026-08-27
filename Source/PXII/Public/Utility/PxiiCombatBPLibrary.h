// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Combat/PxiiProjectileBase.h"
#include "GameplayTagContainer.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Character/PxiiCharacter.h"
#include "Components/PxiiAimComponent.h"
#include "Components/PxiiAimAssistComponent.h"
#include "Engine/DataAsset.h"
#include "PxiiCombatBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiCombatBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	// Retrieves all actors within a radius
	UFUNCTION(BlueprintCallable, meta = (WorldContext = WorldContextObject), meta = (ToolTip = "Fetches all actors within an area with filtering"))
	static TArray<AActor*> GetActorsWithinRadius(FVector Origin, float Radius, UObject * WorldContextObject, EFactionType FactionType, bool ShowDebug);

	UFUNCTION(BlueprintCallable)
	static void StartProjectileTrace(APxiiCharacter* character, FHitInformation& TraceInformation, bool processDamage = false, FName MuzzleSocketName = "Muzzle", bool drawDebugTrace = true);

	static void ProcessTraceHit(APxiiCharacter* character, FHitResult HitResult, bool processDamage, bool DrawTrace);
	static void ProcessTraceMissed(APxiiCharacter* character, FHitInformation TraceInfo, bool DrawTrace);
	
	static bool DoCameraTrace(APxiiCharacter* character, float TraceDistance, FHitResult& HitResult, FVector& TraceEnd, bool DrawTrace);

	UFUNCTION(BlueprintPure, Category="PXII|World")
	static bool IsGameWorld(UObject* WorldContextObject);
	
	static bool DoSocketTrace(APxiiCharacter* character, FName socketName, FVector aimPoint, FHitInformation& HitResult, bool DrawTrace);

	static bool GetCameraViewPoint(APxiiCharacter* character, FVector& OutLoc, FVector& OutDir);

	UFUNCTION(BlueprintPure)
	static bool GetWeaponSocketTransform(APxiiCharacter* character, FName MuzzleSocketName, FTransform& OutTransform);
	
	UFUNCTION(BlueprintCallable)
	static void PauseAI(AAIController* AICon, const FString& Reason = "Paused")
	{
		if (AICon && AICon->BrainComponent)
		{
			if (auto* BTComp = Cast<UBehaviorTreeComponent>(AICon->BrainComponent))
			{
				BTComp->PauseLogic(FString::Printf(TEXT("%s"), *Reason));
			}
		}
	}
	UFUNCTION(BlueprintCallable)
	static void ResumeAI(AAIController* AICon, const FString& Reason = "Resumed")
	{
		if (AICon && AICon->BrainComponent)
		{
			if (auto* BTComp = Cast<UBehaviorTreeComponent>(AICon->BrainComponent))
			{
				BTComp->ResumeLogic(FString::Printf(TEXT("%s"), *Reason));
			}
		}
	}

	UFUNCTION(BlueprintCallable)
	static void RegisterHitEffect(AActor* SourceActor, AActor* TargetActor, const FHitResult& result, float Magnitude, EHitEffectType HitEffectType);
	
	UFUNCTION(BlueprintCallable)
	static void RegisterHitStatusEffectOnly(AActor* SourceActor, AActor* TargetActor, const FHitResult& result, float Magnitude, EHitEffectType HitEffectType);

	UFUNCTION(BlueprintCallable,Category="PXII|Combat")
	static TArray<AActor*> MultiSphereTraceTargetChain(UObject* WorldContextObject, AActor* Owner, FVector Origin, FVector Direction, float Distance,
		float SphereRadius, int32 MaxTraces, TSubclassOf<AActor> TargetActorClass, ETraceTypeQuery TraceChannel, bool bDrawDebug=false, bool bErrorLog=false);
};
