// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Combat/PxiiProjectileBase.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "Engine/DeveloperSettings.h"
#include "Data/PxiiTags.h"
#include "ProjectileSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogProjectileSubsystem, Log, All);

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
	UFUNCTION(BlueprintCallable)
	void TestCppFunc();

	UFUNCTION(BlueprintCallable, Category = "Projectile Subsystem")
	void InitializePool(TSoftClassPtr<APxiiProjectileBase> ProjectileClass, UPARAM(meta = (Categories = "Pxii.Projectiles")) FGameplayTag ClassTag, int32 InitialPoolSize, int32 MaxPoolSize);

	UFUNCTION(BlueprintPure, Category = "Projectile Subsystem")
	TSoftClassPtr<APxiiProjectileBase> GetSoftProjectileClassByTag (UPARAM(meta = (Categories = "Pxii.Projectiles")) FGameplayTag InTag);

	UFUNCTION(BlueprintPure, Category = "Projectile Subsystem")
	TMap<FGameplayTag, FProjectilePool> GetProjectilesMap() { return ProjectilesMap; }

	UFUNCTION(BlueprintCallable, Category = "Projectile Subsystem")
	APxiiProjectileBase* SpawnProjectileFromPool(UPARAM(meta = (Categories = "Pxii.Projectiles")) FGameplayTag ProjectileTag, FTransform SpawnTransform);

// Debug
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Subsystem | Debug")
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY(BlueprintReadOnly , Category = "Projectile Subsystem | Debug")
	UUserWidget* CreatedHUD;

	UPROPERTY(BlueprintAssignable, Category = "Projectile Subsystem | Debug")
	FOnAmmoCountUpdate OnAmmoCountUpdate;

	UPROPERTY(BlueprintAssignable, Category = "Projectile Subsystem | Debug")
	FOnProjectileReturnPool OnProjectileReturnPool;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Subsystem | Debug")
	bool bPrintDebugLog { false };
// Debug

private:
	TMap<FGameplayTag, FProjectilePool> ProjectilesMap;

	TObjectPtr<APxiiProjectileBase> FindAvalaibleProjectileInPool (FGameplayTag InTag);
	void AddMoreProjectilesToPoolAsNeeded(FProjectilePool* ProjectilePool, FGameplayTag ProjectileTag, const int32 NumOfProjectileToAdd);
};


#pragma region Developer Settings
// Developer settings section
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Projectile Subsystem Settings"))
class PXII_API UProjectileSubsystemDeveloperSettings : public UDeveloperSettings 
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Projectile Classes Soft Reference", meta = (ForceInlineRow, Categories = "Pxii.Projectiles"))
	TMap<FGameplayTag, TSoftClassPtr<APxiiProjectileBase>> ProjectileClassesMap;
};
#pragma endregion


#pragma region Debug
// For debug
namespace Debug 
{
	bool bPrintDebugLog { false };
	void Print(UClass* InClass, const FString& InMsg, FColor InColor);
	void Print(const FString& InMsg, FColor InColor);
}
#pragma endregion