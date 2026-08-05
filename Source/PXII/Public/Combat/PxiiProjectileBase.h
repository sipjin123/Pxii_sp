// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PxiiProjectileBase.generated.h"

UCLASS()
class PXII_API APxiiProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	

	APxiiProjectileBase();

	UPROPERTY(BlueprintReadOnly, Category = "Projectile")
	AActor* InstigatorActor;

	UPROPERTY(BlueprintReadOnly, Category = "Projectile")
	AActor* WeaponOwner;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<UGameplayEffect> DamageGE;
	
	UPROPERTY()
	UAbilitySystemComponent* InstigatorASC;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|Damage")
	float ExplosionRadius = 0.0f;

	FORCEINLINE bool GetIsInUse();

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
	void ApplyDamage(AActor* HitActor, const FHitResult& Hit);
	
	virtual void ApplyDamage_Implementation(AActor* HitActor, const FHitResult& Hit);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
	void SpawnImpactEffects(const FHitResult& Hit);
	
	virtual void SpawnImpactEffects_Implementation(const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
	void SpawnTrailEffects();
	
	virtual void SpawnTrailEffects_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
	void InitializeProjectile(float BaseDamage, const FVector& Direction, float Speed, AActor* InInstigator, AActor* InWeaponOwner);

	void InitializeProjectile_Implementation(float BaseDamage, const FVector& Direction, float Speed, AActor* InInstigator, AActor* InWeaponOwner);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
	void ApplyDamageEffectToActor(AActor* TargetActor, const FHitResult& result);
	
	void ApplyDamageEffectToActor_Implementation(AActor* TargetActor, const FHitResult& result);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Projectile")
	void SetDamageClass(UGameplayEffect* effect);
	
	void SetDamageClass_Implementation(UGameplayEffect* effect);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileRoot;
	
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
	TObjectPtr<UNiagaraSystem> ImpactEffect;

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
	
private:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UGameplayEffect> OverrideDamageClass;

	UPROPERTY(EditDefaultsOnly)
	bool overrideEffect = false;

	UPROPERTY(EditDefaultsOnly)
	float BaseDMG;
	
	bool bIsInUse = false;

	FTimerHandle LifetimeTimerHandle;
};
