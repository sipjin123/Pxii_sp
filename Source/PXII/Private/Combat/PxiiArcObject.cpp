// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PxiiCharacter.h"
#include "Combat/PxiiArcObject.h"

// Sets default values
APxiiArcObject::APxiiArcObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APxiiArcObject::InitializeArc(const TArray<FVector>& InArcPoints, float InTravelTime)
{
	ArcPoints = InArcPoints;
	TravelTime = InTravelTime;

	if (ArcPoints.Num() >= 2)
	{
		CurrentIndex = 0;
		ElapsedTime = 0.f;

		// Total time to traverse the entire arc
		SetActorLocation(ArcPoints[0]);
	}
}

void APxiiArcObject::BeginPlay()
{
	Super::BeginPlay();
}

void APxiiArcObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ArcPoints.Num() < 2) return;

	ElapsedTime += DeltaTime;
	float Alpha = ElapsedTime / TravelTime;

	if (Alpha >= 1.f)
	{
		SetActorLocation(ArcPoints.Last());
		HitTarget.Broadcast(GetActorLocation());
		//Destroy(); // ✅ reached end, destroy
		return;
	}

	// Compute index along arc
	float SegmentFloat = Alpha * (ArcPoints.Num() - 1);
	int32 SegmentIndex = FMath::Clamp(FMath::FloorToInt(SegmentFloat), 0, ArcPoints.Num() - 2);
	float SegmentAlpha = SegmentFloat - SegmentIndex;

	FVector NewLocation = FMath::Lerp(ArcPoints[SegmentIndex], ArcPoints[SegmentIndex + 1], SegmentAlpha);
	SetActorLocation(NewLocation);
}