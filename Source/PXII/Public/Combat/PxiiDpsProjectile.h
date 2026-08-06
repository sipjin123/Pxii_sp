#pragma once
#include "PxiiProjectileBase.h"
#include "Enum/PxiiDamageType.h"
#include "PxiiDpsProjectile.generated.h"

USTRUCT(BlueprintType)
struct FDpsData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	EHitEffectType HitEffectType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	float Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	float Period;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	float DamagePerPeriod;
};

UCLASS()
class PXII_API APxiiDpsProjectile : public APxiiProjectileBase
{
	GENERATED_BODY()

public:

	virtual void ApplyDamageEffectToActor_Implementation(AActor* TargetActor, const FHitResult& result) override;

	UFUNCTION(BlueprintCallable)
	virtual void SetDpsData(const FDpsData& data);

	virtual void ReturnProjecileToPool_Implementation() override;
	
private:

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	bool isDpsSet = false;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FDpsData DPSData;

};

