// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/PxiiEnemyType.h"
#include "GameFramework/Actor.h"
#include "PxiiCombatDirector.generated.h"

USTRUCT(BlueprintType)
struct FPxiiSquadMember
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> Actor=nullptr;

	UPROPERTY(BlueprintReadOnly)
	EEnemy EnemyType=EEnemy::Brawler;

	UPROPERTY(BlueprintReadOnly)
	float CombatValue=1.f;
};

UCLASS()
class PXII_API APxiiCombatDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APxiiCombatDirector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable,Category="PXII|Combat|Squad")
	void RegisterSquadMember(AActor* Member,EEnemy EnemyType);

	UFUNCTION(BlueprintCallable,Category="PXII|Combat|Squad")
	void UnregisterSquadMember(AActor* Member);

	UFUNCTION(BlueprintPure,Category="PXII|Combat|Squad")
	float GetRemainingSquadStrength() const;

	UFUNCTION(BlueprintPure,Category="PXII|Combat|Squad")
	float GetInitialSquadStrength() const;
	
	UPROPERTY(BlueprintReadOnly,Category="PXII|Combat|Squad")
	TArray<FPxiiSquadMember> SquadMembers;

	UFUNCTION(BlueprintPure,Category="PXII|Combat|Squad")
	float GetEnemyCombatValue(EEnemy EnemyType) const;
};
