// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"
#include "Components/PxiiAimComponent.h"
#include "Data/PxiiHitFeedbackData.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Data/ImpactData.h"
#include "PxiiProjectileBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnReturnToPool, APxiiProjectileBase*);

USTRUCT()
struct FProjectileHitEffects
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category= "Projectile")
	TArray<TSubclassOf<UGameplayEffect>> TargetHitGE;

	UPROPERTY(EditDefaultsOnly, Category= "Projectile")
	TArray<TSubclassOf<UGameplayEffect>> PlayerHitGE;
};

UCLASS()
class PXII_API APxiiProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	

	APxiiProjectileBase();

	FOnReturnToPool OnReturnToPool;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile")
	AActor* InstigatorActor;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile")
	AActor* WeaponOwner;

	UPROPERTY(EditAnywhere, Category = "Distance Trigger")
	float TriggerDistance = 1000.f;

	UPROPERTY(VisibleAnywhere, Category = "Distance Trigger")
	FVector StartingLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Distance Trigger")
	bool bDistanceTriggered = false;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<UGameplayEffect> DamageGE;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	FProjectileHitEffects ProjectileHitEffect;
	
	UPROPERTY()
	UAbilitySystemComponent* InstigatorASC;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	float ExplosionRadius = 0.0f;

	bool GetIsInUse();

	virtual void BeginPlay() override;
	
	void SetIsInUse(bool InIsInUse);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit);

	void OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnLifetimeExpired();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
	void ReturnProjecileToPool();
	virtual void ReturnProjecileToPool_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
	void ApplyDamage(AActor* HitActor, const FHitResult& Hit);
	
	virtual void ApplyDamage_Implementation(AActor* HitActor, const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
	void SpawnHitEffect(const FHitResult& HitResult);

	void SpawnHitEffect_Implementation(const FHitResult& HitResult);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
	void SpawnImpactEffects(const FHitResult& Hit);
	
	virtual void SpawnImpactEffects_Implementation(const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
	void SpawnTrailEffects();
	
	virtual void SpawnTrailEffects_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
	void InitializeProjectile(float BaseDamage, const FHitInformation& Direction, float Speed, AActor* InInstigator, AActor* InWeaponOwner);

	virtual void InitializeProjectile_Implementation(float BaseDamage, const FHitInformation& Direction, float Speed, AActor* InInstigator, AActor* InWeaponOwner);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
	void ApplyDamageEffectToActor(AActor* TargetActor, const FHitResult& result);
	
	virtual void ApplyDamageEffectToActor_Implementation(AActor* TargetActor, const FHitResult& result);

	virtual void ApplyPlayerProjectileHitEffect();
	
	virtual void ApplyTargetProjectileHitEffect(AActor* TargetActor);
	
	UFUNCTION(BlueprintPure, Category = "Projectile")
	float GetDamage(const FHitResult& result);

	UFUNCTION(BlueprintPure)
	float GetBulletBonusDamage();

	void SetProjectileTag(FGameplayTag inTag);

	FGameplayTag GetPoolTag();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	float Lifetime = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	bool bIsDestroyAfterHit = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	bool EnableDamageFalloff = false;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPxiiHitFeedbackData> HitFeedback;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImpactData> ImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> TrailEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|VFX", meta = (AllowPrivateAccess = "true"))
	FName TrailSocket;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|VFX", meta = (AllowPrivateAccess = "true"))
	FVector TrailLocationOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|VFX", meta = (AllowPrivateAccess = "true"))
	FRotator TrailRotationOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|VFX", meta = (AllowPrivateAccess = "true"))
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, Category = "Distance Trigger")
	float CheckInterval = 0.05f;

	FTimerHandle DistanceCheckTimerHandle;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CheckDistanceTrigger();
	
	virtual void CheckDistanceTrigger_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDistanceReached();

	virtual void OnDistanceReached_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SpawnAOEImpact();

	virtual void SpawnAOEImpact_Implementation();

private:

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UNiagaraComponent* TrailComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	bool DrawDebug;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	float BaseDMG;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage", meta=(AllowPrivateAccess = "true"))
	float BulletBonusDamage = 0.0f;
	
	bool bIsInUse = false;

	FHitInformation TraceInformation;
	
	FTimerHandle LifetimeTimerHandle;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TEnumAsByte<EPhysicalSurface> TargetSurfaceType;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	EHitFeedbackType HitFeedbackType = EHitFeedbackType::Standard;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FGameplayTag PoolTag;

	EPhysicalSurface GetSurfaceType(const FHitResult& hit);
};
