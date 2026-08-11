// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Character/PxiiCharacter.h"
#include "Character/PxiiCharacterBase.h"
#include "Combat/PxiiDpsProjectile.h"
#include "Enum/PxiiDamageType.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PxiiCombatComponent.generated.h"


class APxiiCharacterBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDisplayHitVfx, FVector, HitCoords);

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class PXII_API UPxiiCombatComponent:public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPxiiCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category="Combat")
	void TriggerProjectileTraceArc(FVector ImpactNormal, FVector ImpactLocation, float ArcHeight);

	UFUNCTION(BlueprintCallable, Category="Combat")
	void TriggerProjectileTrace(FVector ImpactNormal, FVector ImpactLocation, bool drawTrace = false);
	
	// Line Trace Debug Parameters
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool ShowDebugLine;
	
	UFUNCTION(BlueprintCallable)
	void InitializeHitTrace(FVector SocketLoc, float LengthOverride, EHitEffectType NewHitEffectType);

	UPROPERTY(BlueprintReadWrite)
	EHitEffectType HitEffectType;
	
	UFUNCTION(BlueprintCallable)
	void ProcessHitTrace(FVector SocketLoc);

	UFUNCTION(BlueprintCallable)
	void ProcessHitTraceLogic(FVector StartLoc, FVector EndLoc, bool drawTrace = false);

	UFUNCTION(BlueprintCallable)
	void FinalizeHitTraceLogic();

	UFUNCTION(BlueprintCallable)
	void EndHitTrace();

	UFUNCTION(BlueprintCallable)
	void ProcessDepthSlash(FVector EndLoc);

	UFUNCTION(BlueprintCallable)
	void ProcessUnitDamage(AActor* TargetUnit, FVector HitLoc, float Damage, EDamageSource DamageSource);

	UFUNCTION(BlueprintCallable)
	void ProcessDPSDamage(AActor* TargetUnit, FVector HitLoc, const FDpsData& Damage, EDamageSource DamageSource);
	
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> HitTracedActors;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> AbilityTrackedActors;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FHitResult> SlashDataArray;
	
	UPROPERTY(BlueprintReadOnly)
	APawn* CharacterRef;

	UPROPERTY(BlueprintReadWrite)
	UAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadWrite)
	int32 TraceIndex;
	
	UPROPERTY(BlueprintReadWrite)
	FVector PreviousSocketLocation;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
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

	UPROPERTY(EditAnywhere)
	FVector BoxExtent = FVector(50.f, 5.f, 2.f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float LengthExtension = 0.f;
};
