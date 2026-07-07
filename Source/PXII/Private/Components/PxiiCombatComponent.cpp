// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PxiiCombatComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Utility/PxiiDebugTraceBPLibrary.h"

// Sets default values for this component's properties
UPxiiCombatComponent::UPxiiCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPxiiCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Warning, TEXT("CombatComponent Init Cpp"));
}


// Called every frame
void UPxiiCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPxiiCombatComponent::TriggerProjectileTraceArc(FVector ImpactNormal, FVector ImpactLocation, float ArcHeight)
{

	
}

void UPxiiCombatComponent::TriggerProjectileTrace(FVector ImpactNormal, FVector ImpactLocation)
{
	UPxiiDebugTraceBPLibrary::DrawDebugSphereSimple(this, ImpactLocation, 50.f, FLinearColor::Blue, 3.f);
	/*
	FTransform MuzzleTransform = SMWeapon->GetSocketTransform(MuzzleSocketName);
	FTransform SocketTransform = MuzzleTransform;
	FVector ToTarget = TargetLoc - SocketTransform.GetLocation();
	FRotator TargetRotation = ToTarget.Rotation();

	FActorSpawnParameters SpawnParams;
	if (ShowDebugLine)
	{
		DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TargetLoc, FColor::Red, false, ShowDebugLineDuration, 0, .9f);
	}

	// Dev override switches here for Debugging
	const  bool SpawnDecal = true;
	const bool ReleaseVfx = true;
	
	if (ReleaseVfx)
	{
		if (ProjectileClass)
			APXIIProjectile* SpawnedProjectile = GetWorld()->SpawnActor<APXIIProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
		if (MuzzleVFX)
			GetWorld()->SpawnActor<AActor>(MuzzleVFX, SocketTransform.GetLocation(), TargetRotation, SpawnParams);

		// TODO(BURLIN): Check if this is still needed, PXIIProjectile.cpp already has VFX on collide
		if (HitVFX)
			GetWorld()->SpawnActor<AActor>(HitVFX, TargetLoc, TargetRotation, SpawnParams);

		if (SpawnDecal){
			// TODO(BURLIN): Check if randomization is necessary
			int32 RandomNumber = FMath::RandRange(5, 10); 
			FVector DecalSize = FVector(RandomNumber, RandomNumber, RandomNumber);
			float LifeSpan = 7.0f;

			// Spawn the Decal
			UGameplayStatics::SpawnDecalAtLocation(
					  GetWorld(),
					  BulletHoleDecal,
					  DecalSize,
					  TargetLoc,
					  ImpactNormal.Rotation(),
					  LifeSpan
				  );
		}
	}*/
}

