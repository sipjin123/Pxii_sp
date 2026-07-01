// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Combat/PxiiProjectileBase.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "Engine/DeveloperSettings.h"
#include "ProjectileSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogProjectileSubsystem, Log, All);

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
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class PXII_API UProjectileSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;

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


#pragma region GameplayTags
// Gameplay tags section
namespace Projectiles
{
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_Projectiles_Basic);
}
#pragma endregion


#pragma region Debug
// For debug
namespace Debug 
{
	void Print(UClass* InClass, const FString& InMsg, FColor InColor);
	void Print(const FString& InMsg, FColor InColor);
}
#pragma endregion