#pragma once
#include "NiagaraSystem.h"
#include "Engine/DataAsset.h"
#include "ImpactData.generated.h"

USTRUCT(BlueprintType)
struct FImpactEffectData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "Impact")
	UNiagaraSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, Category = "Impact")
	TArray<TObjectPtr<USoundBase>> ImpactSfx;

	UPROPERTY(EditAnywhere, Category = "Impact")
	UMaterialInterface* DecalMaterial = nullptr;

	UPROPERTY(EditAnywhere, Category = "Impact")
	FVector DecalSize = FVector(4.0f, 4.0f, 4.0f);

	UPROPERTY(EditAnywhere, Category = "Impact")
	float DecalLifetime = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Impact")
	float SoundVolumeMultiplier = 1.0f;
};

UCLASS(BlueprintType)
class PXII_API UImpactData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Impact")
	TMap<TEnumAsByte<EPhysicalSurface>, FImpactEffectData> SurfaceEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Impact")
	FImpactEffectData DefaultEffect;

	const FImpactEffectData& GetEffectForSurface(EPhysicalSurface SurfaceType) const;

};
