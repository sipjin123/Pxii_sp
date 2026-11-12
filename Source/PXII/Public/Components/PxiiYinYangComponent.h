#pragma once
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Data/PxiiYinYangData.h"
#include "GAS/PxiiAbilitySystemComponent.h"
#include "PxiiYinYangComponent.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiYinYangComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ApplyArmbladeCost();
	void ApplyArmbladeCost_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ApplyCost(EWeaponResourceType WeaponType, float cost);
	void ApplyCost_Implementation(EWeaponResourceType WeaponType, float cost);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ApplyYinYangStressEffect(EWeaponResourceType WeaponType);
	void ApplyYinYangStressEffect_Implementation(EWeaponResourceType WeaponType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisableYinYangStressEffect(EWeaponResourceType WeaponType);
	void DisableYinYangStressEffect_Implementation(EWeaponResourceType WeaponType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ApplyYinYangOverwhelmEffect(EWeaponResourceType WeaponType);
	void ApplyYinYangOverwhelmEffect_Implementation(EWeaponResourceType WeaponType);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisableYinYangOverwhelmEffect(EWeaponResourceType WeaponType);
	void DisableYinYangOverwhelmEffect_Implementation(EWeaponResourceType WeaponType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ApplyYinYangFullEffect(EWeaponResourceType WeaponType);
	void ApplyYinYangFullEffect_Implementation(EWeaponResourceType WeaponType);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisableYinYangFullEffect(EWeaponResourceType WeaponType);
	void DisableYinYangFullEffect_Implementation(EWeaponResourceType WeaponType);

	FGameplayEffectSpecHandle GetCostEffect(EWeaponResourceType WeaponType, FGameplayEffectContextHandle context);
	FGameplayEffectSpecHandle GetStressEffect(EWeaponResourceType WeaponType, FGameplayEffectContextHandle context);
	FGameplayEffectSpecHandle GetOverwhelmEffect(EWeaponResourceType WeaponType, FGameplayEffectContextHandle context);
	FGameplayEffectSpecHandle GetFullEffect(EWeaponResourceType WeaponType, FGameplayEffectContextHandle context);

	UFUNCTION(BlueprintPure)
	UPxiiYinYangData* GetYinYangData();
	
protected:

	UFUNCTION(BlueprintPure)
	UPxiiAbilitySystemComponent* GetASC();
	
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPxiiYinYangData> YinYangData;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag YinTag;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag YangTag;

	UPROPERTY()
	TObjectPtr<UPxiiAbilitySystemComponent> ASC;

	UPROPERTY()
	FActiveGameplayEffectHandle StressGEHandle;

	UPROPERTY()
	FActiveGameplayEffectHandle OverwhelmGEHandle;

	UPROPERTY()
	FActiveGameplayEffectHandle FullGEHandle;
};
