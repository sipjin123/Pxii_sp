// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/PxiiDynamicCurveTracker.h"

// Sets default values
APxiiDynamicCurveTracker::APxiiDynamicCurveTracker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APxiiDynamicCurveTracker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APxiiDynamicCurveTracker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsValid(HomingTarget) || !ActiveObject)
	{
		return;
	}
	HomingElapsedTime+=DeltaTime;
	const float Alpha=FMath::Clamp(HomingElapsedTime/HomingDuration,0.f,1.f);
	const FVector NewLocation=CalculateHomingCurve(Alpha);
	SetActorLocation(NewLocation);
	if (Alpha>=.95f)
	{
		HitTarget.Broadcast(GetActorLocation());
		// Handle impact
	}
}

void APxiiDynamicCurveTracker::ActivateObject()
{
	HomingOrigin=GetActorLocation();
	ActiveObject = true;
}

void APxiiDynamicCurveTracker::InitializeHoming(AActor* Target, float Duration, float InCurveHeight)
{
	if (!IsValid(Target))
	{
		return;
	}
	HomingTarget=Target;
	HomingOrigin=GetActorLocation();
	HomingDuration=Duration;
	CurveHeight=InCurveHeight;
	HomingElapsedTime=0.f;
	const FVector Direction=(Target->GetActorLocation()-HomingOrigin).GetSafeNormal();
	CurveDirection=Direction.RotateAngleAxis(FMath::RandRange(-CurveAngle,CurveAngle),FVector::UpVector);
}

FVector APxiiDynamicCurveTracker::CalculateHomingCurve(float Alpha) const
{
	if (!IsValid(HomingTarget))
	{
		return GetActorLocation();
	}
	bool useCurve = true;
	if(useCurve)
	{
		const FVector TargetLocation=HomingTarget->GetActorLocation();
		const float Distance=FVector::Dist(HomingOrigin,TargetLocation);
		const FVector ControlPoint=HomingOrigin+(CurveDirection*(Distance*0.5f))+FVector::UpVector*CurveHeight;
		const FVector A=FMath::Lerp(HomingOrigin,ControlPoint,Alpha);
		const FVector B=FMath::Lerp(ControlPoint,TargetLocation,Alpha);
		return FMath::Lerp(A,B,Alpha);
	}
	else
	{
		const FVector TargetLocation=HomingTarget->GetActorLocation();
		const FVector Direction=(TargetLocation-HomingOrigin).GetSafeNormal();
		const FVector ControlPoint=HomingOrigin+(Direction*(FVector::Dist(HomingOrigin,TargetLocation)*0.5f))+FVector::UpVector*CurveHeight;
		const FVector A=FMath::Lerp(HomingOrigin,ControlPoint,Alpha);
		const FVector B=FMath::Lerp(ControlPoint,TargetLocation,Alpha);
		return FMath::Lerp(A,B,Alpha);
	}
}

