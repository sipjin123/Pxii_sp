// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PxiiCharacter.h"
#include "Components/PxiiCombatComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PxiiPlayerCombatComponent.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPxiiYinYangType : uint8
{
	Yin,
	Yang
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Invalid,
	Armblade,
	Blaster
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class PXII_API UPxiiPlayerCombatComponent : public UPxiiCombatComponent
{
	GENERATED_BODY()

public:
	
	UPxiiPlayerCombatComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideActiveWeapon();
	virtual void HideActiveWeapon_Implementation();

	void CheckActiveWeapon(EWeaponType weaponType);
	
protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	void OnAbilityActivated(UGameplayAbility* ActivatedAbility);
	void OnAbilityEnded(UGameplayAbility* ActivatedAbility);

private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UGameplayAbility>> RangeAttackClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UGameplayAbility>> MeleeAttackClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<APxiiCharacter> OwnerCharacter;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float ShowWeaponTimer = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	EWeaponType ActiveWeapon = EWeaponType::Invalid;
	
	FTimerHandle LastAttackTimer;
};
