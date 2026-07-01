// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PxiiCharacter.h"
#include "Character/PxiiCharacterBase.h"
#include "Components/PxiiCombatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PxiiPlayerCombatComponent.generated.h"

/**
 * 
 */

class APxiiCharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDisplayHitVfx, FVector, HitCoords);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class PXII_API UPxiiPlayerCombatComponent : public UPxiiCombatComponent
{
	GENERATED_BODY()
public:
	UPxiiPlayerCombatComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void InitializeHitTrace(FVector SocketLoc);

	UFUNCTION(BlueprintCallable)
	void ProcessHitTrace(FVector SocketLoc);

	UFUNCTION(BlueprintCallable)
	void ProcessHitTraceLogic(FVector StartLoc, FVector EndLoc);

	UFUNCTION(BlueprintCallable)
	void FinalizeHitTraceLogic();

	UFUNCTION(BlueprintCallable)
	void EndHitTrace();

	UFUNCTION(BlueprintCallable)
	void ProcessDepthSlash(FVector EndLoc);

	UFUNCTION(BlueprintCallable)
	void ProcessUnitDamage(AActor* TargetUnit, FVector HitLoc);

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> HitTracedActors;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> AbilityTrackedActors;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FHitResult> SlashDataArray;
	
	UPROPERTY(BlueprintReadOnly)
	APxiiCharacter* CharacterRef;

	UPROPERTY(BlueprintReadWrite)
	UAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadWrite)
	int32 TraceIndex;
	
	UPROPERTY(BlueprintReadWrite)
	FVector PreviousSocketLocation;
	
	UPROPERTY(BlueprintReadWrite)
	float TraceDuration = 3.f;

	UPROPERTY(BlueprintReadWrite)
	bool HasHitReaction;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool UseDepthSlash;

	UPROPERTY(BlueprintReadWrite)
	bool HasValidHit;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDisplayHitVfx DisplayHitVfx;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Debug")
	bool LogSlashLogic;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Debug")
	bool LogHits;
	
	UPROPERTY(EditAnywhere, Category="Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTrace = EDrawDebugTrace::None;
};
