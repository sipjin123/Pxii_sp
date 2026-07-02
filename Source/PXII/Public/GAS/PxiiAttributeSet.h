// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "PxiiAttributeSet.generated.h"

/**
 * 
*/
#define ATTRIBUTE_ASSESSORS(ClassName,PropertyName)\
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName)\
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)

UCLASS(BlueprintType)
class PXII_API UPxiiAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

public:
	UPxiiAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData Health;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, Mana);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, MaxMana);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData Ammo;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, Ammo);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData MaxAmmo;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, MaxAmmo);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, Strength);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData Agility;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, Agility);

	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, Intelligence);

};
