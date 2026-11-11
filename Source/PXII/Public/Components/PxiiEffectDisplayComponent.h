#pragma once
#include "GAS/PxiiAbilitySystemComponent.h"
#include "GAS/PxiiGameplayEffectBase.h"
#include "PxiiEffectDisplayComponent.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiEffectDisplayComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;;

	UFUNCTION(BlueprintPure)
	UPxiiAbilitySystemComponent* GetASC();

	UFUNCTION()
	void HandleEffectApplied(UAbilitySystemComponent* Source, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle);

	UFUNCTION()
	void HandleEffectRemoved(const FActiveGameplayEffect& ActiveEffect);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowEffectText(FActiveGameplayEffectHandle Handle, FEffectDetails GEDetails, bool isInstant);
	virtual void ShowEffectText_Implementation(FActiveGameplayEffectHandle Handle, FEffectDetails GEDetails, bool isInstant);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemoveEffectText(FActiveGameplayEffectHandle handle);
	virtual void RemoveEffectText_Implementation(FActiveGameplayEffectHandle handle);

protected:

	UPROPERTY(BlueprintReadOnly)
	TMap<FActiveGameplayEffectHandle, FText> ActiveEffectTexts;
	
	UPROPERTY(BlueprintReadOnly)
	UPxiiAbilitySystemComponent* ASC;

	FDelegateHandle AppliedHandle;
	FDelegateHandle RemovedHandle;
};
