// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PxiiEnemySubsystem.generated.h"

/**
 * 
 */

DECLARE_LOG_CATEGORY_EXTERN(LogEnemySubsystem, Log, All);
UCLASS(Abstract, Blueprintable)
class PXII_API UPxiiEnemySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
public:
	UPxiiEnemySubsystem();

	UFUNCTION(BlueprintCallable, Category="AI")
	APawn* SpawnAI(UObject* WorldContextObject, TSubclassOf<APawn> AIClass, UBehaviorTree* BehaviorTree, const FTransform& SpawnTransform);

	UFUNCTION(BlueprintCallable)
	void DespawnAI(APawn* AI);

	UPROPERTY(BlueprintReadWrite)
	TArray<APawn*> SpawnedAI;
	
	UFUNCTION(BlueprintPure)
	const TArray<APawn*>& GetSpawnedAI() const
	{
		return SpawnedAI;
	}
};
