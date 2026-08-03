// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/PxiiEnemySubsystem.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"


DEFINE_LOG_CATEGORY(LogEnemySubsystem);

void UPxiiEnemySubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
}

UPxiiEnemySubsystem::UPxiiEnemySubsystem()
{
}

APawn* UPxiiEnemySubsystem::SpawnAI(UObject* WorldContextObject, TSubclassOf<APawn> AIClass,
	UBehaviorTree* BehaviorTree, const FTransform& SpawnTransform)
{
	if (!WorldContextObject || !AIClass)
	{
		return nullptr;
	}

	APawn* SpawnedPawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(
		WorldContextObject, AIClass, BehaviorTree,
		SpawnTransform.GetLocation(), SpawnTransform.Rotator(),
		true
	);

	if (SpawnedPawn)
	{
		SpawnedAI.Add(SpawnedPawn);
	}

	return SpawnedPawn;
}

void UPxiiEnemySubsystem::DespawnAI(APawn* AI)
{
	if (!AI) return;
	SpawnedAI.Remove(AI);
	AI->Destroy();
}
