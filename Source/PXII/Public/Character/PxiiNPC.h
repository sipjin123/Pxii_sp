// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "PxiiCharacterBase.h"
#include "GameFramework/Character.h"
#include "PxiiNPC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHitEffectType, EHitEffectType, EffectType, int32, Magnitude);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttackState, bool, IsEnabled, int32, Payload);
UCLASS()
class PXII_API APxiiNPC : public ACharacter, public IAbilitySystemInterface, public IPxiiCombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APxiiNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void OnStaggerMeterChanged(const FOnAttributeChangeData& Data);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UPxiiAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly,Category="Components")
	TSubclassOf<UPxiiCombatComponent> CombatComponentClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	TObjectPtr<UPxiiCombatComponent> CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapons")
	TObjectPtr<USkeletalMeshComponent> MeshRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	bool IsAttacking;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	const UPxiiAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual UPxiiCombatComponent* GetCombatComponent_Implementation() const override;
	virtual const UPxiiAttributeSet* GetAttributeSet_Implementation() const override;
	virtual void EnableAttackState_Implementation() override;
	virtual void DisableAttackState_Implementation() override;

	UPROPERTY(BlueprintAssignable, Category="Combat")
	FHitEffectType OnHitEffectType;
	
	UPROPERTY(BlueprintAssignable, Category="Combat")
	FAttackState OnAttackState;
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess="true"), Category = "PXII|AbilitySystem")
	const class UPxiiAttributeSet* AttributeSet;
};
