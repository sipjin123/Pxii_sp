#pragma once
#include "PxiiAimComponent.h"
#include "Components/ActorComponent.h"
#include "PxiiAimAssistComponent.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiAimAssistComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPxiiAimAssistComponent();
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	FVector GetAdjustedAimDirection(const FVector& rawAimDirection, const FVector& cameraLocation) const;

	UFUNCTION(BlueprintCallable)
	FVector2D GetMagnetizedRecticlePosition(const FVector2D& rawRetScreenPosition, float deltaTime);

	UFUNCTION(BlueprintCallable)
	FVector GetLeadPredictedAimPoint(float projectileSpeed) const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ConeAssistDegress = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AssistStrengthAtCenter = 0.6f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AssistMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ReticleStickyRadius = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AssistLerpSpeed = 12.0f;

private:
	UPROPERTY(EditDefaultsOnly)
	UPxiiAimComponent* AimComponent;

	FVector2D SmoothedRetPosition;
	bool ReticleInitialized = false;

	bool GetCurrentTargetAimPoint(FVector& OutLocation) const;
};
