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

FVector UPxiiMathBPLibrary::GetLocationRightOfActor(const AActor* Actor, float Distance)
{
	if (!Actor) return FVector::ZeroVector;
	return Actor->GetActorLocation() + Actor->GetActorRightVector() * Distance;
}

FVector UPxiiMathBPLibrary::GetLocationInFrontOfActorCam(const AActor* Actor, const FRotator& CameraRotation, float Distance)
{
	if (!Actor)
		return FVector::ZeroVector;

	FRotator Rotation = Actor->GetActorRotation();
	Rotation.Yaw = CameraRotation.Yaw;
	const FVector Forward = Rotation.Vector();
	return Actor->GetActorLocation() + Forward * Distance;
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

bool UPxiiMathBPLibrary::GetRandomValidLocationBehindActor(const AActor* Actor, FVector Offset, int32 TraceCount, float DistanceBehind, float SphereRadius, float ConeAngle, FVector& OutLocation)
{
	OutLocation = FVector::ZeroVector;

	if (!Actor || TraceCount <= 0 || DistanceBehind <= 0.f || ConeAngle <= 0.f)
	{
		return false;
	}

	UWorld* World = Actor->GetWorld();
	if (!World)
	{
		return false;
	}

	const FVector Origin = Actor->GetActorLocation() + Offset;
	const FVector Backward = -Actor->GetActorForwardVector();
	const FRotator BackwardRotation = Backward.Rotation();

	TArray<FVector> ValidLocations;
	ValidLocations.Reserve(TraceCount);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Actor);

	const float HalfConeAngle = ConeAngle * 0.5f;
	for (int32 i = 0; i < TraceCount; ++i)
	{
		const float Alpha = TraceCount == 1 ? 0.5f : static_cast<float>(i) / (TraceCount - 1);
		const float Angle = FMath::Lerp(-HalfConeAngle, HalfConeAngle, Alpha);

		const FVector Direction = BackwardRotation.RotateVector(FVector::ForwardVector).RotateAngleAxis(Angle, FVector::UpVector);
		const FVector CandidateLocation = Origin + Direction * DistanceBehind;

		FHitResult HitResult;

		const bool bHit = World->SweepSingleByChannel(HitResult, CandidateLocation, CandidateLocation, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(SphereRadius), QueryParams);

		DrawDebugSphere(World, CandidateLocation, SphereRadius, 12, bHit ? FColor::Red : FColor::Green, false, 5.0f, 0, 2.0f);
		DrawDebugLine(World, Origin, CandidateLocation, bHit ? FColor::Red : FColor::Green, false, 5.0f, 0, 1.0f);

		if (bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("[CRS] ----------------------------Hit: %s"), HitResult.GetActor() ? *HitResult.GetActor()->GetName() : TEXT("None"));
			continue;
		}

		// No obstruction, trace downward to find the ground.
		const FVector GroundTraceStart = CandidateLocation + FVector(0.f, 0.f, 50.f);
		const FVector GroundTraceEnd = CandidateLocation - FVector(0.f, 0.f, 500.f);

		FHitResult GroundHit;

		const bool bGroundHit = World->LineTraceSingleByChannel(GroundHit, GroundTraceStart, GroundTraceEnd, ECC_Visibility, QueryParams);

		DrawDebugLine(World, GroundTraceStart, GroundTraceEnd, bGroundHit ? FColor::Blue : FColor::Yellow, false, 5.0f, 0, 1.0f);

		if (!bGroundHit)
		{
			continue;
		}

		const FVector GroundLocation = GroundHit.Location;

		// Check whether the ground location is on the NavMesh.
		FNavLocation NavLocation;
		const UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);

		if (!NavSystem || !NavSystem->ProjectPointToNavigation(GroundLocation, NavLocation))
		{
			DrawDebugSphere(World, GroundLocation, SphereRadius, 12, FColor::Orange, false, 5.0f, 0, 2.0f);
			continue;
		}

		DrawDebugSphere(World, GroundLocation, SphereRadius, 12, FColor::Green, false, 5.0f, 0, 2.0f);

		ValidLocations.Add(GroundLocation);
	}

	if (ValidLocations.Num() == 0)
	{
		return false;
	}

	OutLocation = ValidLocations[FMath::RandRange(0, ValidLocations.Num() - 1)];
	return true;
}

TArray<FVector> UPxiiMathBPLibrary::GenerateShotgunTrajectories(
FVector Origin,
FVector ForwardDirection,
float Distance,
int32 PelletCount,
float SpreadAngleDegrees
)
{
	TArray<FVector> Trajectories;

	if (PelletCount <= 0 || Distance <= 0.f)
	{
		return Trajectories;
	}

	ForwardDirection.Normalize();

	Trajectories.Reserve(PelletCount);

	const float SpreadRadians = FMath::DegreesToRadians(SpreadAngleDegrees);

	for (int32 i = 0; i < PelletCount; ++i)
	{
		const FVector PelletDirection =
			FMath::VRandCone(ForwardDirection, SpreadRadians);

		const FVector EndPoint =
			Origin + (PelletDirection * Distance);

		Trajectories.Add(EndPoint);
	}

	return Trajectories;
}

TArray<FVector> UPxiiMathBPLibrary::GenerateRandomPointsInSphere(FVector Origin, FVector ForwardDirection, float Radius, float Flatness,int32 NumPoints, float MinSpacing)
{
	TArray<FVector> Points;
	if (NumPoints <= 0 || Radius <= 0.f) return Points;

	ForwardDirection = ForwardDirection.GetSafeNormal();

	if (ForwardDirection.IsNearlyZero())
	{
		ForwardDirection = FVector::ForwardVector;
	}

	Flatness = FMath::Clamp(Flatness, 0.f, 1.f);
	MinSpacing = FMath::Max(0.f, MinSpacing);

	Points.Reserve(NumPoints);

	const int32 MaxAttempts = NumPoints * 1000;
	int32 Attempts = 0;

	while (Points.Num() < NumPoints && Attempts < MaxAttempts)
	{
		++Attempts;

		// Generate random point inside a unit sphere.
		FVector LocalPoint = FMath::VRand();

		// Uniform distribution through the volume.
		LocalPoint *= FMath::Pow(FMath::FRand(), 1.f / 3.f);

		// Convert to radius.
		LocalPoint *= Radius;

		/*
		 * Flatten along the ForwardDirection.
		 *
		 * Separate the point into:
		 *   Forward component
		 *   Perpendicular component
		 */
		const float ForwardDistance = FVector::DotProduct(LocalPoint, ForwardDirection);
		const FVector ForwardComponent = ForwardDirection * ForwardDistance;
		const FVector PerpendicularComponent = LocalPoint - ForwardComponent;

		// Reduce depth while preserving width/height.
		LocalPoint = PerpendicularComponent + ForwardComponent * Flatness;

		const FVector Candidate = Origin + LocalPoint;

		bool bValid = true;

		for (const FVector& ExistingPoint : Points)
		{
			if (FVector::DistSquared(Candidate, ExistingPoint) <
				FMath::Square(MinSpacing))
			{
				bValid = false;
				break;
			}
		}

		if (bValid)
		{
			Points.Add(Candidate);
		}
	}

	return Points;
}

TArray<FVector> UPxiiMathBPLibrary::GenerateRandomPointsInRing(FVector Origin, FVector ForwardDirection, float InnerRadius, float OuterRadius, int32 NumPoints, float MinSpacing)
{
	TArray<FVector> Points;

	if (NumPoints <= 0 || OuterRadius <= 0.f)
	{
		return Points;
	}

	ForwardDirection.Z = 0.f;
	ForwardDirection.Normalize();

	if (ForwardDirection.IsNearlyZero())
	{
		ForwardDirection = FVector::ForwardVector;
	}

	const FVector Right = FVector::CrossProduct(FVector::UpVector, ForwardDirection).GetSafeNormal();

	const float InnerSq = FMath::Square(FMath::Max(0.f, InnerRadius));
	const float OuterSq = FMath::Square(FMath::Max(InnerRadius, OuterRadius));

	Points.Reserve(NumPoints);

	for (int32 i = 0; i < NumPoints; ++i)
	{
		const bool bRightHemisphere = (i % 2) == 0;

		// Right: -90° to 90°
		// Left :  90° to 270°
		const float Angle = bRightHemisphere
			? FMath::FRandRange(-HALF_PI, HALF_PI)
			: FMath::FRandRange(HALF_PI, PI + HALF_PI);

		const float Radius = FMath::Sqrt(FMath::FRandRange(InnerSq, OuterSq));

		const FVector Offset =
			(ForwardDirection * FMath::Cos(Angle) +
			 Right * FMath::Sin(Angle)) * Radius;

		Points.Add(Origin + Offset);
	}

	return Points;
}

void UPxiiMathBPLibrary::GenerateSimpleArc(const bool bDrawDebug, const bool bUseRandom,
	const UObject* WorldContextObject, const FVector& Start, const FVector& End, float Height, int32 NumPoints,
	TArray<FVector>& OutPoints, FVector ArcDirection)
{
	OutPoints.Reset();

	const FColor DebugColor = FColor::Green;   // line/point color
	const float LifeTime    = .5f;             // seconds visible
	const float Thickness   = 2.f;             // line thickness

	if (NumPoints < 2) return;

	// Normalize arc direction (default to up if invalid)
	if (ArcDirection.IsNearlyZero())
	{
		ArcDirection = FVector::UpVector;
	}
	
	if (bUseRandom)
	{
		do
		{
			ArcDirection = FMath::VRand();   // random unit vector
		}
		while (ArcDirection.Z < 0.f);        // reject downward directions
	}
	else
	{
		ArcDirection = ArcDirection.GetSafeNormal();
	}

	// Control point halfway between Start/End, shifted along ArcDirection
	FVector Mid = (Start + End) * 0.5f + ArcDirection * Height;

	FVector Prev = Start;

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);

	for (int32 i = 0; i < NumPoints; i++)
	{
		float t = (float)i / (float)(NumPoints - 1);
		FVector Point =
			(1 - t) * (1 - t) * Start +
			2 * (1 - t) * t * Mid +
			t * t * End;

		OutPoints.Add(Point);

		if (bDrawDebug && i > 0 && World)
		{
			DrawDebugLine(World, Prev, Point, DebugColor, false, LifeTime, 0, Thickness);
			DrawDebugPoint(World, Point, 6.f, DebugColor, false, LifeTime);
		}

		Prev = Point;
	}
}
