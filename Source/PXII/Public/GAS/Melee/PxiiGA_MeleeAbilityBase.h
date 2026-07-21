// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/PxiiGA_OneshotBase.h"
#include "PxiiGA_MeleeAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiGA_MeleeAbilityBase : public UPxiiGA_OneshotBase
{
	
	GENERATED_BODY()

public:
	explicit UPxiiGA_MeleeAbilityBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	virtual void OnCompleted();

	UFUNCTION()
	virtual void OnBlendOut();

	UFUNCTION()
	virtual void OnInterrupted();

	UFUNCTION()
	virtual void OnCancelled();

	UFUNCTION(BlueprintImplementableEvent, Category = "PXII|Ability|Dodge", DisplayName = "OnCompleted", meta = (ScriptName = "OnCompleted"))
	void K2_OnCompleted();							   
													   
	UFUNCTION(BlueprintImplementableEvent, Category = "PXII|Ability|Dodge", DisplayName = "OnBlendOut", meta = (ScriptName = "OnBlendOut"))
	void K2_OnBlendOut();							   
													   
	UFUNCTION(BlueprintImplementableEvent, Category = "PXII|Ability|Dodge", DisplayName = "OnInterrupted", meta = (ScriptName = "OnInterrupted"))
	void K2_OnInterrupted();						   
													   
	UFUNCTION(BlueprintImplementableEvent, Category = "PXII|Ability|Dodge", DisplayName = "OnCancelled", meta = (ScriptName = "OnCancelled"))
	void K2_OnCancelled();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> MontageToPlay;
};
