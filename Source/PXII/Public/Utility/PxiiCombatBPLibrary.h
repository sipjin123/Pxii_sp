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
#include "PxiiCombatBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiCombatBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Combat | BP Function Library")
	static TSoftClassPtr<APxiiProjectileBase> GetSoftProjectileClassByTag(UPARAM(meta = (Categories = "Pxii.Projectiles")) FGameplayTag InTag);

	UFUNCTION(BlueprintCallable)
	static void StartProjectileTrace(APxiiCharacter* character, FName MuzzleSocketName = "Muzzle");

	static bool DoCameraTrace(APxiiCharacter* character, float TraceDistance, FHitResult& HitResult, FVector& TraceEnd, bool DrawTrace);

	static bool DoSocketTrace(APxiiCharacter* character, FName socketName, FVector aimPoint, FHitInformation& HitResult, bool DrawTrace);

	static bool GetCameraViewPoint(APxiiCharacter* character, FVector& OutLoc, FVector& OutDir);
	
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
};
