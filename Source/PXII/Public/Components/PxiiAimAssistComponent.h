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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim | Aim Direction", meta = (ToolTip = "How wide a cone around your aim the game checks for a nearby target to help you track. Increase = helps from further off-target (easier); decrease = only kicks in when you're already close (harder)."))
	float ConeAssistDegrees = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim | Aim Direction", meta = (ToolTip = "How strongly your aim gets pulled toward a target when you're dead-on it. Fades out toward the edge of the cone. Increase = stronger, more noticeable pull; decrease = lighter, more subtle pull."))
	float AssistStrengthAtCenter = 0.15f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim | Aim Direction", meta = (ToolTip = "Master strength dial for all aim assist (pull, magnetism, and slowdown together).	Increase = everything feels stronger; decrease = everything feels weaker. Set to 0 to turn assist off entirely. "))
	float AssistMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim | Magnetized Reticle", meta = (ToolTip = "How close (in screen pixels) your reticle needs to be to a target before it starts sticking to it. Increase = sticks from further away (easier to catch); decrease = only sticks when you're basically already on it."))
	float ReticleStickyRadius = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim | Magnetized Reticle", meta = (ToolTip = "How quickly the reticle glides toward a sticky target once caught. Increase = snappier, more immediate stick; decrease = slower, smoother glide."))
	float AssistLerpSpeed = 12.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim | Aim Slowdown", meta = (ToolTip = "If on, slowdown only happens while you have a target locked — not just from being near one."))
	bool RequireActiveLock = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim | Aim Slowdown", meta = (ToolTip = "Shapes how slowdown ramps up as your reticle gets closer to the target. Increase = slowdown mostly felt right on top of the target; decrease = slowdown felt earlier, more gradually."))
	float SlowdownFalloffExp = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim | Aim Slowdown", meta = (ClampMin = "0.0", ClampMax = "1.0", ToolTip = "How much your aim slows down when your reticle is right on the target (1 = no slowdown, 0 = fully stops).	Increase = less slowdown, aim stays fast; decrease = more slowdown, easier to hold on target."))
	float SlowDownFactor = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aim | Aim Slowdown", meta = (ToolTip = "How close (in screen pixels) your reticle needs to be to a target before aim slowdown kicks in. Increase = slowdown starts from further away; decrease = only kicks in when you're nearly on target. "))
	float SlowdownRadius = 50.0f;
	
private:
	UPROPERTY(EditDefaultsOnly)
	UPxiiAimComponent* AimComponent;

	FVector2D SmoothedRetPosition;
	bool ReticleInitialized = false;

	bool GetCurrentTargetAimPoint(FVector& OutLocation) const;
};
