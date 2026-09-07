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
	void ApplyCost(UResourceGenerationCost* cost);
	void ApplyCost_Implementation(UResourceGenerationCost* cost);

	UFUNCTION(BlueprintPure)
	float GetFinalCost(EWeaponResourceType resourceType, UResourceGenerationCost* costData) const;

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
	
};
