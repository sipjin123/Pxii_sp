#pragma once
#include "PxiiBlasterData.generated.h"

UCLASS(BlueprintType)
class PXII_API UPxiiBlasterData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MinimumDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MagazineSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool EnableDistanceFalloff;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DistanceFalloffStart;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DistanceFalloffEnd;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CriticalMultiplier;
};
