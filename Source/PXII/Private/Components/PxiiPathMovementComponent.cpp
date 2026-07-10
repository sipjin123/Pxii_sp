// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PxiiPathMovementComponent.h"

#include "MoverComponent.h"
#include "Structs/PxiiLayeredMovePath.h"

// Sets default values for this component's properties
UPxiiPathMovementComponent::UPxiiPathMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPxiiPathMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPxiiPathMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (false)
	{
		if (!bMoving || Path.Num() < 2)
			return;

		Elapsed += DeltaTime;

		Progress = FMath::Clamp(Elapsed / Duration, 0.f, 1.f);

		const float PathPosition = Progress * (Path.Num() - 1);

		CurrIndex = FMath::Clamp(FMath::FloorToInt(PathPosition), 0, Path.Num() - 2);

		const float Alpha = PathPosition - CurrIndex;

		CurrentLocation = FMath::Lerp(Path[CurrIndex], Path[CurrIndex + 1], Alpha);

		if (Progress >= 1.f)
		{
			CurrentLocation = Path.Last();
			bMoving = false;
		}
	}
	else
	{
		if (!bMoving || Path.Num() < 2)
		{
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("BB_LOG:: GOOOO: %d"), CurrIndex);

		ElapsedDistance += MoveSpeed * DeltaTime;

		if (ElapsedDistance >= TotalDistance)
		{
			CurrentLocation = Path.Last();
			CurrIndex = Path.Num() - 2;
			bMoving = false;
			{
				return;
			}
		}

		while (CurrIndex < Path.Num() - 2 && ElapsedDistance > CumulativeDistance[CurrIndex + 1])
		{
			++CurrIndex;
		}

		const float SegmentStart = CumulativeDistance[CurrIndex];
		const float SegmentEnd = CumulativeDistance[CurrIndex + 1];
		const float SegmentAlpha = (ElapsedDistance - SegmentStart) / (SegmentEnd - SegmentStart);

		CurrentLocation = FMath::Lerp(Path[CurrIndex], Path[CurrIndex + 1], SegmentAlpha);
	}
}

void UPxiiPathMovementComponent::StartPath(const TArray<FVector>& InPath, float InDuration)
{
	if (false)
	{
		Path = InPath;
		Duration = FMath::Max(InDuration, KINDA_SMALL_NUMBER);
		Elapsed = 0.f;
		Progress = 0.f;
		bMoving = Path.Num() >= 2;
		CurrentLocation = bMoving ? Path[0] : FVector::ZeroVector;
	}
	else
	{
		Path = InPath;
		MoveSpeed = InDuration;
		CurrIndex = 0;
		ElapsedDistance = 0.f;
		bMoving = Path.Num() > 1;

		CumulativeDistance.Empty();
		CumulativeDistance.Add(0.f);

		float Total = 0.f;

		for (int32 i = 1; i < Path.Num(); ++i)
		{
			Total += FVector::Distance(Path[i - 1], Path[i]);
			CumulativeDistance.Add(Total);
		}

		TotalDistance = Total;
		CurrentLocation = Path[0];
		
	}
}

void UPxiiPathMovementComponent::StopPath()
{
	bMoving = false;
	UE_LOG(LogTemp, Log, TEXT("BB_LOG:: STOP NOW XX"));
}