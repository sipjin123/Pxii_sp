#pragma once
#include "PxiiArmbladeEffectData.generated.h"

UENUM()
enum class EHitImpact : uint8
{
	Light,
	Medium,
	Heavy,
	Finisher,
};

USTRUCT(BlueprintType)
struct FImpactSoundEffect
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "Impact", meta = (ClampMin = -2.0f, ClampMax = 2.0f))
	TArray<TObjectPtr<USoundBase>> ImpactSfx;
	
	UPROPERTY(EditAnywhere, Category = "Impact", meta = (ClampMin = -2.0f, ClampMax = 2.0f))
	float MinPitchMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Impact", meta = (ClampMin = -2.0f, ClampMax = 2.0f))
	float MaxPitchMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Impact", meta = (ClampMin = 0.0f, ClampMax = 10.0f))
	float MinVolumeMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Impact", meta = (ClampMin = 0.0f, ClampMax = 10.0f))
	float MaxVolumeMultiplier = 1.0f;
};

USTRUCT(BlueprintType)
struct FWeaponSlashSfx
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<USoundBase>> WeaponSfx;

	UPROPERTY(EditAnywhere, Category = "Impact")
	float MinPitchMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Impact")
	float MaxPitchMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Impact")
	float MinVolumeMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Impact")
	float MaxVolumeMultiplier = 1.0f;
};

USTRUCT(BlueprintType)
struct FHitStopData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact")
	float HitStopDuration = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact")
	float HitStopDilation = 0.0;
};


UCLASS(BlueprintType)
class PXII_API UPxiiArmbladeEffectData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWeaponSlashSfx ArmbladeSlashSfx;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EHitImpact, FHitStopData> HitStopMap;

	FImpactSoundEffect DefaultImpactSFX;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Impact")
	TMap<TEnumAsByte<EPhysicalSurface>, FImpactSoundEffect> ArmbladeImpactSFX;
};
