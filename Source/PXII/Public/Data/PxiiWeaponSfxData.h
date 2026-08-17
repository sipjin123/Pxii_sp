#pragma once
#include "Engine/DataAsset.h"
#include "Sound/SoundBase.h"
#include "PxiiWeaponSfxData.generated.h"

USTRUCT(BlueprintType)
struct FSfxData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> Sfx;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float VolumeMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Pitch = 1.0f;
};


UCLASS(BlueprintType)
class PXII_API UPxiiWeaponSfxData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSfxData> FireSfx;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSfxData MechanicalClick;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSfxData DistantFireSfx;
};
