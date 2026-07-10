// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/PxiiMathBPLibrary.h"

#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

FVector UPxiiMathBPLibrary::GetLocationInFrontOfActor(const AActor* Actor, float Distance)
{
	if (!Actor) return FVector::ZeroVector;
	return Actor->GetActorLocation() + Actor->GetActorForwardVector() * Distance;
}

TArray<FVector> UPxiiMathBPLibrary::GenerateArcPath(const UObject* WorldContextObject, const FVector& Start, const FVector& End, float Height,
	int32 NodeCount, bool bDrawDebug, float DebugDuration, float drawRadius)
{
	TArray<FVector> Points;

	NodeCount = FMath::Max(NodeCount, 2);
	Points.Reserve(NodeCount);

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	
	for (int32 i = 0; i < NodeCount; ++i)
	{
		const float Alpha = (float)i / (NodeCount - 1);

		// Linear interpolation between start and end.
		FVector Point = FMath::Lerp(Start, End, Alpha);

		// Parabolic arc.
		const float ArcHeight = 4.f * Height * Alpha * (1.f - Alpha);

		Point.Z += ArcHeight;

		Points.Add(Point);
		if (bDrawDebug && World)
		{
			DrawDebugSphere(World, Point, drawRadius, 12, FColor::Green, false, DebugDuration);

			if (i > 0)
			{
				DrawDebugLine(World, Points[i - 1], Point, FColor::Yellow, false, DebugDuration, 0, 2.f);
			}
		}
	}

	return Points;
}
