// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/PxiiProjectileBase.h"
#include "Subsystem/ProjectileSubsystem.h"

// Sets default values
APxiiProjectileBase::APxiiProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
}

// Called when the game starts or when spawned
void APxiiProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APxiiProjectileBase::SetIsInUse(bool InIsInUse)
{
	bIsInUse = InIsInUse;

	this->SetActorEnableCollision(bIsInUse);
	this->SetActorHiddenInGame(!bIsInUse);
	this->SetActorTickEnabled(bIsInUse);

	// Always make sure timer is clear, no matter true or false
	if (LifetimeTimerHandle.IsValid() && GetWorldTimerManager().IsTimerActive(LifetimeTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(LifetimeTimerHandle);
	}

	// Only start timer when needed
	if (bIsInUse)
	{
		GetWorldTimerManager().SetTimer(
			LifetimeTimerHandle,
			[this]() {
				this->SetIsInUse(false);

				Debug::Print(ThisClass::StaticClass(), FString::Printf(TEXT("Projectile returned to pool")), FColor::Blue);
			},
			Lifetime,
			false
		);
	}
}

void APxiiProjectileBase::OnProjectileHit()
{
	if (bIsDestroyAfterHit)
	{
		SetIsInUse(false);
	}
}

