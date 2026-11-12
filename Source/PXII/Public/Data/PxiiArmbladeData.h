#pragma once
#include "PxiiArmbladeData.generated.h"

UCLASS(BlueprintType)
class PXII_API UPxiiArmbladeData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseDamage = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CriticalMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BonusDamage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackSpeed = 1.0f;
};
