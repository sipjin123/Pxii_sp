// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PxiiAbilitySystemComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTempAbilityComp, Log, All);
/**
 * 
 */
UCLASS(ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class PXII_API UPxiiAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable,Category="Ability")
	void GrantAbilityByRow(FName RowName);

	UFUNCTION(BlueprintCallable,Category="Ability")
	void GrantAllAbilities();

	UPROPERTY(EditAnywhere)
	bool LogAbilityInit;
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Ability")
	UDataTable* AbilityDataTable;
};
