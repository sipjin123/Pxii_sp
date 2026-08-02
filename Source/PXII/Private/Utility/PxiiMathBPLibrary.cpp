// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/PxiiMathBPLibrary.h"

#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "Engine/Engine.h"
#include "NavFilters/NavigationQueryFilter.h"

FVector UPxiiMathBPLibrary::GetLocationInFrontOfActor(const AActor* Actor, float Distance)
{
	if (!Actor) return FVector::ZeroVector;
	return Actor->GetActorLocation() + Actor->GetActorForwardVector() * Distance;
}

FVector UPxiiMathBPLibrary::GetLocationTopOfActor(const AActor* Actor, float Distance)
{
	if (!Actor) return FVector::ZeroVector;
	return Actor->GetActorLocation() + Actor->GetActorUpVector() * Distance;
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

FVector UPxiiMathBPLibrary::GetOffsetPositionTowardOrigin(const FVector& Origin, const FVector& Target, float Distance)
{
	FVector Direction = (Origin - Target).GetSafeNormal();
	return Target + (Direction * Distance);
}

bool UPxiiMathBPLibrary::GetRandomNavigablePointInRing(UObject* WorldContextObject, const FVector& Origin,
	float InnerRadius, float OuterRadius, TSubclassOf<UNavigationQueryFilter> FilterClass, FVector& OutLocation,
	int32& Attempts, int32 MaxAttempts, bool bRequireReachable, bool bDrawDebug, float Duration)
{
	if (!WorldContextObject) return false;

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return false;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
	if (!NavSys) return false;

	ANavigationData* NavData = NavSys->GetDefaultNavDataInstance();
	if (!NavData) return false;

	FSharedConstNavQueryFilter NavFilter = UNavigationQueryFilter::GetQueryFilter(*NavData, FilterClass);

	if (bDrawDebug)
	{
		DrawDebugSphere(World, Origin, InnerRadius, 32, FColor::Red, false, Duration, 0, 2.f);
		DrawDebugSphere(World, Origin, OuterRadius, 32, FColor::Green, false, Duration, 0, 2.f);
	}
	Attempts = 0;
	for (int32 i = 0; i < MaxAttempts; ++i)
	{
		Attempts++;
		FNavLocation RandomPoint;

		const bool bFoundPoint = bRequireReachable
			? NavSys->GetRandomReachablePointInRadius(Origin, OuterRadius, RandomPoint, NavData, NavFilter)
			: NavSys->GetRandomPointInNavigableRadius(Origin, OuterRadius, RandomPoint, NavData, NavFilter);

		
		if (bFoundPoint)
		{
			float DistSq = FVector::DistSquared2D(Origin, RandomPoint.Location);

			if (DistSq >= InnerRadius * InnerRadius)
			{
				OutLocation = RandomPoint.Location;

				if (bDrawDebug)
				{
					DrawDebugPoint(World, OutLocation, 16.f, FColor::Yellow, false, Duration);
				}

				return true;
			}
		}
	}

	return false;
}
