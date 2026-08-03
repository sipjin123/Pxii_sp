#pragma once
#include "Components/PxiiAimComponent.h"
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

	UFUNCTION(BlueprintCallable)
	float GetAimSlowdownFactor(const FVector2D& reticleScreenPos);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ConeAssistDegrees = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AssistStrengthAtCenter = 0.15f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AssistMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ReticleStickyRadius = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AssistLerpSpeed = 12.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool RequireActiveLock = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SlowdownFalloffExp = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SlowDownFactor = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SlowdownRadius = 50.0f;
	
private:
	UPROPERTY(EditDefaultsOnly)
	UPxiiAimComponent* AimComponent;

	FVector2D SmoothedRetPosition;
	bool ReticleInitialized = false;

	bool GetCurrentTargetAimPoint(FVector& OutLocation) const;
};
