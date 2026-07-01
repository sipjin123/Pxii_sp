// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/PxiiCombatInterface.h"
#include "PxiiCharacterBase.generated.h"

class UPxiiAbilitySystemComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTempBaseCharacter, Log, All);
UCLASS()
class PXII_API APxiiCharacterBase : public ACharacter, public IAbilitySystemInterface, public IPxiiCombatInterface
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	bool IsAttacking;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
protected:
};
