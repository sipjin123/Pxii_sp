#pragma once
#include "Data/PxiiArmbladeEffectData.h"
#include "PxiiImpactMapData.generated.h"

USTRUCT(BlueprintType)
struct FHitstopEffect
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool ApplyHitStop = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EHitImpact ImpactType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool UseCustom = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FHitStopData CustomHitStop;
	
};

UCLASS(BlueprintType)
class PXII_API UPxiiImpactMapData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FHitStopData DefaultImpactData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EHitImpact, FHitStopData> BulletImpactMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EHitImpact, FHitStopData> AbilityHitstopMap;
};
