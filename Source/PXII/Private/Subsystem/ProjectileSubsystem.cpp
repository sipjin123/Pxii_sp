// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ProjectileSubsystem.h"
#include "GameplayTagContainer.h"
#include "Engine/AssetManager.h" 

DEFINE_LOG_CATEGORY(LogProjectileSubsystem);
void UProjectileSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
}

void UProjectileSubsystem::TestCppFunc()
{
	UE_LOG(LogProjectileSubsystem,Warning,TEXT("Test Success"));
}

void UProjectileSubsystem::InitializePool(TSoftClassPtr<APxiiProjectileBase> ProjectileClass, UPARAM(meta = (Categories = "Pxii.Projectiles")) FGameplayTag ClassTag, int32 InitialPoolSize, int32 MaxPoolSize)
{
	if (ProjectileClass.IsNull())
	{
		Debug::Print(ThisClass::StaticClass(), FString::Printf(TEXT("Invalid soft projectile class")), FColor::Red);
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
		}
	);
}

APxiiProjectileBase* UProjectileSubsystem::SpawnProjectileFromPool(UPARAM(meta = (Categories = "Pxii.Projectiles")) FGameplayTag ProjectileTag, FTransform SpawnTransform)
{
	APxiiProjectileBase* SpawnedProjectile = FindAvalaibleProjectileInPool(ProjectileTag);
	if (!SpawnedProjectile) 
	{
		Debug::Print(ThisClass::StaticClass(), FString::Printf(TEXT("Failed to spawn projectile from pool")), FColor::Red);
		return nullptr;
	}

	SpawnedProjectile->SetActorTransform(SpawnTransform);
	SpawnedProjectile->SetIsInUse(true);

	return SpawnedProjectile;
}

TObjectPtr<APxiiProjectileBase> UProjectileSubsystem::FindAvalaibleProjectileInPool(FGameplayTag InTag)
{
	FProjectilePool* FoundProjectilePool = ProjectilesMap.Find(InTag);
	if (!FoundProjectilePool) 
	{
		Debug::Print(ThisClass::StaticClass(), FString::Printf(TEXT("Cant find desired pool in the map")), FColor::Red);
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
			Debug::Print(ThisClass::StaticClass(), FString::Printf(TEXT("Current Projectile Pool Reached Max")), FColor::Red);
			return nullptr;
		}
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

	Debug::Print(ThisClass::StaticClass(), FString::Printf(TEXT("Current Projectile Pool Used Up")), FColor::Red);
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
			ProjectilePool->AvailableProjectiles.AddUnique(SpawnedProjectile);
		}
		else
		{
			break;
		}
	}
}

TSoftClassPtr<APxiiProjectileBase> UProjectileSubsystem::GetSoftProjectileClassByTag(UPARAM(meta = (Categories = "Pxii.Projectiles")) FGameplayTag InTag)
{
	const UProjectileSubsystemDeveloperSettings* ProjectileDevSettings = GetDefault<UProjectileSubsystemDeveloperSettings>();

	if (!ProjectileDevSettings->ProjectileClassesMap.Contains(InTag)) 
	{
		Debug::Print(ThisClass::StaticClass(), FString::Printf(TEXT("Projectile class %s is not assigned in Developer Settings"), *InTag.ToString()), FColor::Red);
		return nullptr;
	}

	return ProjectileDevSettings->ProjectileClassesMap.FindRef(InTag);
}


#pragma region GameplayTags
namespace Projectiles
{
	UE_DEFINE_GAMEPLAY_TAG(Pxii_Projectiles_Basic, "Pxii.Projectiles.Basic");
}
#pragma endregion


#pragma region Debug
namespace Debug
{
	void Print(UClass* InClass, const FString& InMsg, FColor InColor)
	{
		FString ClassName = InClass ? InClass->GetName() : TEXT("None");

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, InColor, FString::Printf(TEXT("[%s]: %s"), *ClassName, *InMsg));
		}

		UE_LOG(LogProjectileSubsystem, Warning, TEXT("[%s]: %s"), *ClassName, *InMsg);
	}

	void Print(const FString& InMsg, FColor InColor)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, InColor, FString::Printf(TEXT("%s"), *InMsg));
		}

		UE_LOG(LogProjectileSubsystem, Warning, TEXT("%s"), *InMsg);
	}
}
#pragma endregion