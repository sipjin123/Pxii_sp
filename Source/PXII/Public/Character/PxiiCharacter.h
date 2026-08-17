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

/**
 * 
 */
UCLASS()
class PXII_API APxiiCharacter : public APxiiCharacterBase
{
	GENERATED_BODY()

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

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ProcessBlasterFired();
	
	void ProcessBlasterFired_Implementation();
	
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetMesh();
	
	UFUNCTION(BlueprintCallable)
	bool GetIsADSEnabled() { return bIsADSActive; }
	UFUNCTION(BlueprintCallable)
	bool GetIsObstructed() { return bIsObstructed; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsADSEnabled(bool isEnabled); 

	void SetIsADSEnabled_Implementation(bool isEnabled);
	
	UFUNCTION()
	void SetIsObstructed(bool bCond) { bIsObstructed = bCond; }

	virtual APxiiWeaponRange* GetWeaponBaseRange_Implementation() override;
	
	virtual APxiiWeaponMelee* GetWeaponBaseMelee_Implementation() override;

	UPROPERTY(BlueprintReadWrite)
	APxiiWeaponRange* CurrentWeaponRanged;
	UPROPERTY(BlueprintReadWrite)
	APxiiWeaponMelee* CurrentWeaponMelee;
	
	/** Signals that a projectile hits a target **/
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Combat")
	FHitTarget TraceTarget;
public:

	virtual void ProcessDamageData_Implementation(AActor* SourceActor, float Damage, float DamageSource) override;
};
