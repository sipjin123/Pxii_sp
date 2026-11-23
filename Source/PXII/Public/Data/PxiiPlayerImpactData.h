#pragma once
#include "PxiiArmbladeEffectData.h"
#include "PxiiPlayerImpactData.generated.h"

UCLASS(BlueprintType)
class UPxiiPlayerImpactData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EHitImpact, TObjectPtr<UCameraShakeAsset>> ImpactCameraShake;
};
