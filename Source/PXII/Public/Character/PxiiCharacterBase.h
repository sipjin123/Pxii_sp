// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/PxiiCombatInterface.h"
#include "PxiiCharacterBase.generated.h"

class UPxiiAimAssistComponent;
class UMoverComponent;
class UPxiiAttributeSet;
class UPxiiAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAggroUpdated, int32, AggroCount);
DECLARE_LOG_CATEGORY_EXTERN(LogTempBaseCharacter, Log, All);
UCLASS()
class PXII_API APxiiCharacterBase : public APawn, public IAbilitySystemInterface, public IPxiiCombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APxiiCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	virtual UPxiiAimAssistComponent* GetAimAssistComponent_Implementation() const override;
	virtual EFactionType GetFaction_Implementation() override;
	virtual void EnableAttackState_Implementation() override;
	virtual void DisableAttackState_Implementation() override;
	
	UPROPERTY(BlueprintReadWrite, Category="Movement")
	TObjectPtr<UMoverComponent> MoverComponent;
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess="true"), Category = "PXII|AbilitySystem")
	const class UPxiiAttributeSet* AttributeSet;

public:
	
	// Interface overrides
	virtual void ReleaseAggro_Implementation();
	virtual void TryAcquireAggro_Implementation(bool& bOutSuccess);
	virtual bool CanGetAggro_Implementation();
	virtual int32 GetAvailableAggroSlots();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EFactionType FactionType;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int MaxAggroSlots = 2;
	
	UPROPERTY(BlueprintReadWrite)
	int CurrentAggroCount = 0;
	
	UFUNCTION()
	void OnRep_AggroCountChanged() { OnAggroUpdated.Broadcast(CurrentAggroCount); }

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAggroUpdated OnAggroUpdated;

	UPROPERTY(BlueprintReadWrite)
	AActor* SpecialActionTarget;
	
	UPROPERTY(EditAnywhere)
	bool LogAggro = true;
};
