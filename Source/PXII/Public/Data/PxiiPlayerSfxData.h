#pragma once
#include "PxiiPlayerSfxData.generated.h"

USTRUCT(BlueprintType)
struct FAttackGruntSfx
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Impact", meta = (ClampMin = -2.0f, ClampMax = 2.0f))
	TArray<TObjectPtr<USoundBase>> GruntSfx;

	UPROPERTY(EditAnywhere, Category = "Impact", meta = (ClampMin = -2.0f, ClampMax = 2.0f))
	float MinPitchMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Impact", meta = (ClampMin = -2.0f, ClampMax = 2.0f))
	float MaxPitchMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Impact", meta = (ClampMin = 0.0f, ClampMax = 10.0f))
	float MinVolumeMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Impact", meta = (ClampMin = 0.0f, ClampMax = 10.0f))
	float MaxVolumeMultiplier = 1.0f;
	
};

UCLASS(BlueprintType)
class PXII_API UPxiiPlayerSfxData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FAttackGruntSfx PlayerAttackGrunt;
};
