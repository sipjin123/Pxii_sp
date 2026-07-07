// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PxiiCharacterBase.h"
#include "Weapon/PxiiWeaponBase.h"
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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsADSActive;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsObstructed;
public:
	
	UFUNCTION()
	bool GetIsADSEnabled() { return bIsADSActive; }
	UFUNCTION()
	bool GetIsObstructed() { return bIsObstructed; }

	UFUNCTION()
	void SetIsADSEnabled(bool bCond) { bIsADSActive = bCond; }
	
	UFUNCTION()
	void SetIsObstructed(bool bCond) { bIsObstructed = bCond; }

	UFUNCTION(BlueprintCallable)
	APxiiWeaponBase* GetWeaponBase();

	/** Signals that a projectile hits a target **/
	UPROPERTY(BlueprintAssignable, Category="Combat")
	FHitTarget TraceTarget;

};
