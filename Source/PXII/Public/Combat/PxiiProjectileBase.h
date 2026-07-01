// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PxiiProjectileBase.generated.h"

UCLASS()
class PXII_API APxiiProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APxiiProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Setter and Getter
	FORCEINLINE bool GetIsInUse() { return bIsInUse; }
	void SetIsInUse(bool InIsInUse);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void OnProjectileHit();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	float Lifetime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	bool bIsDestroyAfterHit { false };

	bool bIsInUse { false };

	FTimerHandle LifetimeTimerHandle;
};
