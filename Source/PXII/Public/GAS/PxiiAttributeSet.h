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
	FGameplayAttributeData Yin;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, Yin);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData MaxYin;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, MaxYin);

	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData Yang;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, Yang);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData MaxYang;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, MaxYang);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData StaggerMeter;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, StaggerMeter);
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData MaxStaggerMeter;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, MaxStaggerMeter);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData Shield;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, Shield);
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, MaxShield);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, Defense);
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData AttackDamage;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, AttackDamage);
	//----------------------------------------------------------------------------------------------------
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, Strength);
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData Agility;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, Agility);

	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, Intelligence);

	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, MovementSpeed);

	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData BaseMovementSpeed;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, BaseMovementSpeed);

	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData MovementSpeedBonus;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, MovementSpeedBonus);

	UPROPERTY(BlueprintReadOnly, Category="Attributes | Primary")
	FGameplayAttributeData SpeedModifier;
	ATTRIBUTE_ASSESSORS(UPxiiAttributeSet, SpeedModifier);
	
};
