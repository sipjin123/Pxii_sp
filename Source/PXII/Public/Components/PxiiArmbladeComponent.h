#pragma once
#include "PxiiHitFeedbackComponent.h"
#include "Data/PxiiArmbladeEffectData.h"
#include "Data/PxiiPlayerSfxData.h"
#include "PxiiArmbladeComponent.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiArmbladeComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPxiiPlayerSfxData> PlayerSfxData;

	UPROPERTY(BlueprintReadOnly)
	EHitDirection HitDirectionType;
	
	UPROPERTY(BlueprintReadOnly)
	EHitImpact HitImpactType;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayAttackGrunt();

	void PlayAttackGrunt_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyHitStop(EHitImpact hitImpact);
	void ApplyHitStop_Implementation( EHitImpact hitImpact);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetHitStop();

	void ResetHitStop_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ShowImpactVfx(const FVector& impactLocation, const FVector& hitDirection);

	void ShowImpactVfx_Implementation(const FVector& impactLocation, const FVector& hitDirection);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayCameraShake(EHitImpact hitImpact);

	void PlayCameraShake_Implementation(EHitImpact hitImpact);
};
