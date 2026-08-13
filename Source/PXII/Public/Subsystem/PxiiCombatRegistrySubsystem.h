// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayEffect.h"
#include "Data/PxiiAbilityData.h"
#include "Enum/PxiiDamageType.h"
#include "GAS/PxiiAbilitySystemComponent.h"
#include "PxiiCombatRegistrySubsystem.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FDamageHit
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> Source;

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> Target;

	UPROPERTY(BlueprintReadWrite)
	float Damage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	uint8 Flags = 0;
	
	UPROPERTY(BlueprintReadWrite)
	EDamageSource DamageSource = EDamageSource::None;
	
	UPROPERTY(BlueprintReadWrite)
	FVector HitCoord = FVector::ZeroVector;
};

struct FQueuedDamage
{
	TWeakObjectPtr<AActor> Source;
	FDamageHit HitData;
};

DECLARE_LOG_CATEGORY_EXTERN(LogCombatRegistrySubsystem, Log, All);
UCLASS(Abstract, Blueprintable)
class PXII_API UPxiiCombatRegistrySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
protected:

	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	
public:

	void GiveGlobalAbilitySet(UPxiiAbilitySystemComponent* targetASC);
	
	// Called by CombatComponent (players & AI)
	UFUNCTION(BlueprintCallable, Category="Combat")
	void EnqueueDamage(AActor* Source, const FDamageHit& Hit);
	void EnsureProcessing();
	void ManualTick();
	void ProcessDamageRegistry(const FQueuedDamage& DamageEntry);
	void ApplySingleDamageEffect(AActor* Source, AActor* Target, float Damage, FVector HitCoords,
								 EHitEffectType HitEffectType, EDamageSource DamageSource);
	void ApplyDPSDamageEffect(AActor* Source, AActor* Target, float Damage, FVector HitCoords, float period,
								float duration, EHitEffectType HitEffectType, EDamageSource DamageSource);

public:
	
	FTimerHandle ProcessTimer;
	bool bProcessingActive = false;

	UPROPERTY(EditAnywhere)
	bool bLogFlow = false;
	UPROPERTY(EditAnywhere)
	bool TraceOrigin = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> GenericDamageEffect;
	TSubclassOf<UGameplayEffect> GetGenericDamageEffect() const { return GenericDamageEffect; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> GenericDPSEffect;
	TSubclassOf<UGameplayEffect> GetGenericDPSEffect() const { return GenericDPSEffect; }
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> KnockbackEffect;
	TSubclassOf<UGameplayEffect> GetKnockbackEffect() const { return KnockbackEffect; }
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> KnockdownEffect;
	TSubclassOf<UGameplayEffect> GetKnockdownEffect() const { return KnockdownEffect; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayEffect> PerfectDodgeEffect;
	TSubclassOf<UGameplayEffect> GetPerfectDodgeEffect() const { return PerfectDodgeEffect; }
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameplayAbility> GA_Stagger;
	TSubclassOf<UGameplayAbility> GetGAStagger() const { return GA_Stagger; } 
	
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPxiiAbilityData> GlobalAbilitySet;
	
	// Simple FIFO queue
	TArray<FQueuedDamage> DamageQueue;

	// Safety limit per tick
	int32 MaxDamagePerTick = 32;
	int32 CurrentTickIndex = 0;
};