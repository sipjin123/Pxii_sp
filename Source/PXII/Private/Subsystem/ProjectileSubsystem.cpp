// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ProjectileSubsystem.h"
#include "GameplayTagContainer.h"
#include "Engine/AssetManager.h" 
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Utility/PxiiCombatBPLibrary.h"
#include "Utility/PXIILogUtility.h"

void UProjectileSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
}

void UProjectileSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	///////////////////
	//// For debug purpose only
	InitializePool(UPxiiCombatBPLibrary::GetSoftProjectileClassByTag(Projectiles::Pxii_Projectiles_Basic), Projectiles::Pxii_Projectiles_Basic, 20, 50);
	///////////////////
}

void UProjectileSubsystem::InitializePool(TSoftClassPtr<APxiiProjectileBase> ProjectileClass, UPARAM(meta = (Categories = "Pxii.Projectiles")) FGameplayTag ClassTag, int32 InitialPoolSize, int32 MaxPoolSize)
{
	if (ProjectileClass.IsNull())
	{
		if(bPrintDebugLog)
		{
			PXII_LOG(ELogCategory::Combat, Warning, TEXT("Invalid soft projectile class"));
		}

		return;
	}

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		ProjectileClass.ToSoftObjectPath(),
		[this, ProjectileClass, ClassTag, InitialPoolSize, MaxPoolSize]() {
			FActorSpawnParameters SpawnParams;
			UClass* LoadedProjectileClass = ProjectileClass.Get();
			FVector Location(0.0f, 0.0f, 100.0f);
			FRotator Rotation(0.0f, 0.0f, 0.0f);

			// When reach here there should be a map exists
			FProjectilePool& ProjectilePool = ProjectilesMap.FindOrAdd(ClassTag);

			ProjectilePool.InitialPoolSize = InitialPoolSize;
			ProjectilePool.MaxPoolSize = MaxPoolSize;

			for (int i = 0; i < InitialPoolSize; i++) 
			{
				// Spawn the projectile into pool
				if (!ProjectilePool.IsMaxAlready())
				{
					APxiiProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<APxiiProjectileBase>(LoadedProjectileClass, Location, Rotation, SpawnParams);
					SpawnedProjectile->SetIsInUse(false);
					ProjectilePool.AvailableProjectiles.AddUnique(SpawnedProjectile);
				}
				else 
				{
					break;
				}
			}

			// Debug
			OnAmmoCountUpdate.Broadcast(ProjectilePool.AvailableProjectiles.Num(), ProjectilePool.AvailableProjectiles.Num());
			// Debug
		}
	);
}

APxiiProjectileBase* UProjectileSubsystem::SpawnProjectileFromPool(UPARAM(meta = (Categories = "Pxii.Projectiles")) FGameplayTag ProjectileTag, FTransform SpawnTransform)
{
	APxiiProjectileBase* SpawnedProjectile = FindAvalaibleProjectileInPool(ProjectileTag);
	if (!SpawnedProjectile) 
	{
		if(bPrintDebugLog)
		{
			PXII_LOG(ELogCategory::Combat, Warning, TEXT("Failed to spawn projectile from pool"));
		}

		return nullptr;
	}

	SpawnedProjectile->SetActorTransform(SpawnTransform);
	SpawnedProjectile->SetIsInUse(true);

	// Debug
	FProjectilePool* FoundProjectilePool = ProjectilesMap.Find(ProjectileTag);
	OnAmmoCountUpdate.Broadcast(FoundProjectilePool->UnusedProjectileCount(), FoundProjectilePool->AvailableProjectiles.Num());
	// Debug

	return SpawnedProjectile;
}

TObjectPtr<APxiiProjectileBase> UProjectileSubsystem::FindAvalaibleProjectileInPool(FGameplayTag InTag)
{
	FProjectilePool* FoundProjectilePool = ProjectilesMap.Find(InTag);
	if (!FoundProjectilePool) 
	{
		if(bPrintDebugLog)
		{
			PXII_LOG(ELogCategory::Combat, Warning, TEXT("Cant find desired pool in the map"));
		}
		return nullptr;
	}

	if (FoundProjectilePool->IsCurrentPoolUsedUp()) 
	{
		if (FoundProjectilePool->AvailableProjectiles.Num() < FoundProjectilePool->MaxPoolSize) 
		{
			AddMoreProjectilesToPoolAsNeeded(FoundProjectilePool, InTag, 1);
		}
		else
		{
			if (bPrintDebugLog)
			{
				PXII_LOG(ELogCategory::Combat, Warning, TEXT("Current Projectile Pool Reached Max"));
			}
		}
			return nullptr;
	}

	for (APxiiProjectileBase* AvailableProjectile : FoundProjectilePool->AvailableProjectiles)
	{
		if (AvailableProjectile->GetIsInUse()) 
		{
			continue;
		}
		else 
		{
			return AvailableProjectile;
		}
	}

	if (bPrintDebugLog)
	{
		PXII_LOG(ELogCategory::Combat, Warning, TEXT("Current Projectile Pool Used Up"));
	}

	return nullptr;
}

void UProjectileSubsystem::AddMoreProjectilesToPoolAsNeeded(FProjectilePool* ProjectilePool, FGameplayTag ProjectileTag, const int32 NumOfProjectileToAdd)
{
	TSoftClassPtr ProjectileClass = UPxiiCombatBPLibrary::GetSoftProjectileClassByTag(ProjectileTag);
	UClass* LoadedProjectileClass = ProjectileClass.Get();

	FActorSpawnParameters SpawnParams;
	FVector Location(0.0f, 0.0f, 100.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < NumOfProjectileToAdd; i++)
	{
		// Spawn the projectile into pool
		if (!ProjectilePool->IsMaxAlready())
		{
			APxiiProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<APxiiProjectileBase>(LoadedProjectileClass, Location, Rotation, SpawnParams);
			SpawnedProjectile->SetIsInUse(false);
			ProjectilePool->AvailableProjectiles.AddUnique(SpawnedProjectile);
		}
		else
		{
			break;
		}
	}
}