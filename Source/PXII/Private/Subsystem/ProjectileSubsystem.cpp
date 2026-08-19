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
	for(TPair<FGameplayTag, TSoftClassPtr<APxiiProjectileBase>>& Pair : DataMap->ProjectileClassesMap)
	{
		
		InitializePool(GetSoftProjectileClassByTag(Pair.Key), Pair.Key, 5, 256);
	}
}

void UProjectileSubsystem::Deinitialize()
{
	if (InitializeHandle.IsValid())
	{
		InitializeHandle->CancelHandle();
		InitializeHandle.Reset();
	}
	Super::Deinitialize();
}

void UProjectileSubsystem::InitializePool(TSoftClassPtr<APxiiProjectileBase> ProjectileClass, UPARAM(meta = (Categories = "Pxii.Projectiles")) FGameplayTag ClassTag, int32 InitialPoolSize, int32 MaxPoolSize)
{
	PXII_LOG(ELogCategory::Projectile, Warning, TEXT("Initializing Pool: %s"), *ClassTag.GetTagName().ToString());
	
	if (ProjectileClass.IsNull())
	{
		if(bPrintDebugLog)
		{
			PXII_LOG(ELogCategory::Combat, Warning, TEXT("Invalid soft projectile class"));
		}

		return;
	}

	InitializeHandle = 
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
					if (SpawnedProjectile)
					{
						SpawnedProjectile->SetProjectileTag(ClassTag);
						SpawnedProjectile->SetIsInUse(false);
						SpawnedProjectile->OnReturnToPool.AddUObject(this, &UProjectileSubsystem::ReturnProjectileToPool);
						ProjectilePool.AvailableProjectiles.AddUnique(SpawnedProjectile);
					}
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

void UProjectileSubsystem::ReturnProjectileToPool(APxiiProjectileBase* projectile)
{
	FProjectilePool* FoundProjectilePool = ProjectilesMap.Find(projectile->GetPoolTag());
	if (!FoundProjectilePool) 
	{
		if(bPrintDebugLog)
		{
			PXII_LOG(ELogCategory::Combat, Warning, TEXT("Cant find desired pool in the map"));
		}
		return;
	}

	PXII_LOG(ELogCategory::Combat, Warning, TEXT("Returning projectile to pool"));

	FoundProjectilePool->UsedProjectiles.Remove(projectile);
	FoundProjectilePool->AvailableProjectiles.AddUnique(projectile);
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

	if (FoundProjectilePool->IsCurrentPoolSizeLow()) 
	{
		if (FoundProjectilePool->GetTotalPoolSize() < FoundProjectilePool->MaxPoolSize) 
		{
			PXII_LOG(ELogCategory::Projectile, Warning, TEXT("Adding more pool item"));

			AddMoreProjectilesToPoolAsNeeded(FoundProjectilePool, InTag, 1);
		}
		else
		{
			if (bPrintDebugLog)
			{
				PXII_LOG(ELogCategory::Combat, Warning, TEXT("Current Projectile Pool Reached Max"));
			}
			return nullptr;
		}
	}

	if(!FoundProjectilePool->AvailableProjectiles.IsEmpty())
	{
		APxiiProjectileBase* target = FoundProjectilePool->AvailableProjectiles[0];
		FoundProjectilePool->UsedProjectiles.AddUnique(target);
		FoundProjectilePool->AvailableProjectiles.Remove(target);
		
		return target; 
	}

	if (bPrintDebugLog)
	{
		PXII_LOG(ELogCategory::Projectile, Warning, TEXT("Current Projectile Pool Used Up"));
	}

	return nullptr;
}

void UProjectileSubsystem::AddMoreProjectilesToPoolAsNeeded(FProjectilePool* ProjectilePool, FGameplayTag ProjectileTag, const int32 NumOfProjectileToAdd)
{
	TSoftClassPtr ProjectileClass = GetSoftProjectileClassByTag(ProjectileTag);
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
			SpawnedProjectile->SetProjectileTag(ProjectileTag);
			SpawnedProjectile->OnReturnToPool.AddUObject(this, &UProjectileSubsystem::ReturnProjectileToPool);
			ProjectilePool->AvailableProjectiles.AddUnique(SpawnedProjectile);
		}
		else
		{
			break;
		}
	}
}

TSoftClassPtr<APxiiProjectileBase> UProjectileSubsystem::GetSoftProjectileClassByTag(FGameplayTag InTag)
{
	if(!DataMap->ProjectileClassesMap.Contains(InTag))
	{
		PXII_LOG(ELogCategory::Combat, Warning, TEXT("[%s]: Projectile class %s is not assigned in %s"), *ThisClass::StaticClass()->GetName(), *InTag.ToString(), *DataMap->GetName());
		return nullptr;
	}

	return DataMap->ProjectileClassesMap.FindRef(InTag);
}