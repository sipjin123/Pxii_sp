#include "Components/PxiiAimAssistComponent.h"
#include "Targeting/Targetable.h"
#include "Utility/PXIILogUtility.h"

UPxiiAimAssistComponent::UPxiiAimAssistComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPxiiAimAssistComponent::BeginPlay()
{
	Super::BeginPlay();

	AimComponent = GetOwner() ? GetOwner()->FindComponentByClass<UPxiiAimComponent>() : nullptr;
	if(!AimComponent)
	{
		PXII_LOG(ELogCategory::Aim, Error, TEXT("AIM COMPONENT NOT FOUND"));
	}
}

FVector UPxiiAimAssistComponent::GetAdjustedAimDirection(const FVector& rawAimDirection, const FVector& cameraLocation) const
{
	FVector targetLocation;
	if(!GetCurrentTargetAimPoint(targetLocation))
	{
		return rawAimDirection;
	}

	FVector rawDirection = rawAimDirection.GetSafeNormal();
	FVector targetDir = (targetLocation - cameraLocation).GetSafeNormal();

	float dotClamped = FMath::Clamp(FVector::DotProduct(rawDirection, targetDir), -1.0f, 1.0f);
	float angle = FMath::RadiansToDegrees(FMath::Acos(dotClamped));

	if(angle > ConeAssistDegrees)
	{
		return rawAimDirection;
	}

	float alpha = FMath::GetMappedRangeValueClamped(FVector2D(0.0f, ConeAssistDegrees),
		FVector2D(AssistStrengthAtCenter, 0.0f), angle) * AssistMultiplier;

	const FVector adjustedAim = FMath::Lerp(rawDirection, targetDir, alpha).GetSafeNormal();
	return adjustedAim;
}

FVector2D UPxiiAimAssistComponent::GetMagnetizedRecticlePosition(const FVector2D& rawRetScreenPosition, float deltaTime)
{
	if(!ReticleInitialized)
	{
		SmoothedRetPosition = rawRetScreenPosition;
		ReticleInitialized = true;
	}

	FVector targetLocation;
	if(!GetCurrentTargetAimPoint(targetLocation))
	{
		SmoothedRetPosition = rawRetScreenPosition;
		return rawRetScreenPosition;
	}

	APawn* owner = Cast<APawn>(GetOwner());
	APlayerController* PC = owner ? Cast<APlayerController>(owner->GetController()) : nullptr;

	if(!PC)
	{
		return rawRetScreenPosition;
	}

	FVector2D screenPos;
	bool projected = PC->ProjectWorldLocationToScreen(targetLocation, screenPos);
	if(!projected)
	{
		SmoothedRetPosition = rawRetScreenPosition;
		return rawRetScreenPosition;
	}

	float screenDist = FVector2D::Distance(rawRetScreenPosition, screenPos);
	FVector2D desiredPos = rawRetScreenPosition;
	if(screenDist <= ReticleStickyRadius)
	{
		const float pull = (1.0f - (screenDist/ReticleStickyRadius)) * AssistMultiplier;
		desiredPos = FMath::Lerp(rawRetScreenPosition, screenPos, pull);
	}

	SmoothedRetPosition = FMath::Vector2DInterpConstantTo(SmoothedRetPosition, desiredPos, deltaTime, AssistLerpSpeed);
	return SmoothedRetPosition;
}

FVector UPxiiAimAssistComponent::GetLeadPredictedAimPoint(float projectileSpeed) const
{
	FVector targetLoc;
	
	if(!GetCurrentTargetAimPoint(targetLoc) || projectileSpeed <= 0.0f)
	{
		return targetLoc;
	}

	AActor* target = AimComponent->GetCurrentTarget();
	FVector velocity = target ? target->GetVelocity() : FVector::ZeroVector;

	APawn* owner = Cast<APawn>(GetOwner());
	FVector actorLoc = owner ? owner->GetActorLocation() : GetOwner()->GetActorLocation();

	float impactTime = FVector::Dist(actorLoc, targetLoc) / projectileSpeed;
	FVector predictedLoc = targetLoc + (velocity * impactTime);

	impactTime = FVector::Dist(actorLoc, predictedLoc) / projectileSpeed;
	predictedLoc = targetLoc + (velocity * impactTime);

	return predictedLoc;
}

float UPxiiAimAssistComponent::GetAimSlowdownFactor(const FVector2D& reticleScreenPos)
{
	if(RequireActiveLock && (!AimComponent || !AimComponent->HasTarget()))
	{
		return 1.0f;
	}

	FVector worldLoc;
	if(!GetCurrentTargetAimPoint(worldLoc))
	{
		return 1.0f;
	}

	APawn* owner = Cast<APawn>(GetOwner());
	APlayerController* PC = owner ? Cast<APlayerController>(owner->GetController()) : nullptr;
	if(!PC)
	{
		return 1.0f;
	}

	FVector2D screenPos;
	if(!PC->ProjectWorldLocationToScreen(worldLoc, screenPos))
	{
		return 1.0f;
	}

	float screenDist = FVector2D::Distance(reticleScreenPos, screenPos);
	if(screenDist >= SlowdownRadius)
	{
		return 1.0f;
	}

	float proximity = 1.0f - (screenDist / SlowdownRadius);
	float shapedProximity = FMath::Pow(proximity, SlowdownFalloffExp);
	float rawFactor = FMath::Lerp(1.0f, SlowDownFactor, shapedProximity);

	return FMath::Lerp(1.0f, rawFactor, AssistMultiplier);
}

bool UPxiiAimAssistComponent::GetCurrentTargetAimPoint(FVector& OutLocation) const
{
	if(!AimComponent || !AimComponent->HasTarget())
	{
		return false;	
	}

	AActor* target = AimComponent->GetCurrentTarget();
	if(!target || !target->GetClass()->ImplementsInterface(UTargetable::StaticClass()))
	{
		return false;
	}

	OutLocation = ITargetable::Execute_GetAimSocketLocation(target);
	return true;
}
