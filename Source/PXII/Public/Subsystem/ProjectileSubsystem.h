// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Combat/PxiiProjectileBase.h"
#include "GameplayTagContainer.h"
#include "Data/PxiiTags.h"
#include "ProjectileSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoCountUpdate, int32, CurrentAmmoCount, int32, CurrentTotalAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileReturnPool);

USTRUCT(BlueprintType)
struct FProjectilePool
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<APxiiProjectileBase>> AvailableProjectiles;

	UPROPERTY(BlueprintReadOnly)
	int32 InitialPoolSize;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxPoolSize;

	bool IsMaxAlready() 
	{
		return AvailableProjectiles.Num() >= MaxPoolSize;
	}

	bool IsCurrentPoolUsedUp() 
	{
		bool bIsUsedUp = false;
		for (APxiiProjectileBase* Projectile : AvailableProjectiles) 
		{
			// If there is one unused projectile then means no used up yet
			if (!Projectile->GetIsInUse()) 
			{
				bIsUsedUp = false;
				break;
			}
			else
			{
				bIsUsedUp = true;
			}
		}

		return bIsUsedUp;
	}

	int32 UnusedProjectileCount() 
	{
		int32 Count = 0;

		for (APxiiProjectileBase* Projectile : AvailableProjectiles)
		{
			if (!Projectile->GetIsInUse()) 
			{
				Count++;
			}
		}

		return Count;
	}
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class PXII_API UProjectileSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	// ~Begin UWorldSubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	// ~End UWorldSubsystem interface

public:
	UFUNCTION(BlueprintCallable, Category = "Projectile Subsystem")
	void InitializePool(TSoftClassPtr<APxiiProjectileBase> ProjectileClass, UPARAM(meta = (Categories = "Pxii.Projectiles")) FGameplayTag ClassTag, int32 InitialPoolSize, int32 MaxPoolSize);

	UFUNCTION(BlueprintPure, Category = "Projectile Subsystem")
	TMap<FGameplayTag, FProjectilePool> GetProjectilesMap() { return ProjectilesMap; }

	UFUNCTION(BlueprintCallable, Category = "Projectile Subsystem")
	APxiiProjectileBase* SpawnProjectileFromPool(UPARAM(meta = (Categories = "Pxii.Projectiles")) FGameplayTag ProjectileTag, FTransform SpawnTransform);

	// Debug
	UPROPERTY(BlueprintAssignable, Category = "Projectile Subsystem | Debug")
	FOnAmmoCountUpdate OnAmmoCountUpdate;

	UPROPERTY(BlueprintAssignable, Category = "Projectile Subsystem | Debug")
	FOnProjectileReturnPool OnProjectileReturnPool;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Subsystem | Debug")
	bool bPrintDebugLog { false };
	// Debug

private:
	TMap<FGameplayTag, FProjectilePool> ProjectilesMap;

	TObjectPtr<APxiiProjectileBase> FindAvalaibleProjectileInPool(FGameplayTag InTag);
	void AddMoreProjectilesToPoolAsNeeded(FProjectilePool* ProjectilePool, FGameplayTag ProjectileTag, const int32 NumOfProjectileToAdd);
};