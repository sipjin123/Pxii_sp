// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PxiiCharacterBase.h"
#include "GAS/PxiiAttributeSet.h"
#include "Weapon/PxiiWeaponBase.h"
#include "Weapon/PxiiWeaponMelee.h"
#include "Weapon/PxiiWeaponRange.h"
#include "GameplayEffectExtension.h"
#include "PxiiCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitTarget, FVector, Location);
DECLARE_LOG_CATEGORY_EXTERN(LogTempCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPxiiOnAttributeChange, float, NewValue);
/**
 * 
 */
UCLASS()
class PXII_API APxiiCharacter : public APxiiCharacterBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FPxiiOnAttributeChange OnMovementSpeedUpdated;

	UPROPERTY(BlueprintAssignable)
	FPxiiOnAttributeChange OnMaxHealthUpdated;

	UPROPERTY(BlueprintAssignable)
	FPxiiOnAttributeChange OnMaxAmmoUpdated;

	UPROPERTY(BlueprintAssignable)
	FPxiiOnAttributeChange OnMaxManaUpdated;

protected:

	TArray<FGameplayAttribute> Attributes =
	{
		UPxiiAttributeSet::GetMaxHealthAttribute(),
		UPxiiAttributeSet::GetMaxAmmoAttribute(),
		UPxiiAttributeSet::GetMaxManaAttribute(),
		UPxiiAttributeSet::GetMovementSpeedAttribute()
	};

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsADSActive;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsObstructed;

	void InitAttributeListener();
	void OnAttributeChanged(FGameplayAttribute Attribute, const FOnAttributeChangeData& Data);

public:
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetMesh();
	
	UFUNCTION(BlueprintCallable)
	bool GetIsADSEnabled() { return bIsADSActive; }
	UFUNCTION(BlueprintCallable)
	bool GetIsObstructed() { return bIsObstructed; }

	UFUNCTION()
	void SetIsADSEnabled(bool bCond) { bIsADSActive = bCond; }
	
	UFUNCTION()
	void SetIsObstructed(bool bCond) { bIsObstructed = bCond; }

	UFUNCTION(BlueprintCallable)
	APxiiWeaponRange* GetWeaponRanged();

	UFUNCTION(BlueprintCallable)
	APxiiWeaponMelee* GetWeaponBaseMelee();

	UPROPERTY(BlueprintReadWrite)
	APxiiWeaponRange* CurrentWeaponRanged;
	UPROPERTY(BlueprintReadWrite)
	APxiiWeaponMelee* CurrentWeaponMelee;
	
	/** Signals that a projectile hits a target **/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Combat")
	FHitTarget TraceTarget;

};
