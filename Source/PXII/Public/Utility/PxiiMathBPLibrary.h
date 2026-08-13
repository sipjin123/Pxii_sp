// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PxiiMathBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiMathBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="PXII|Math")
	static FVector GetLocationInFrontOfActor(const AActor* Actor, float Distance);
	
	UFUNCTION(BlueprintPure, Category="PXII|Math")
	static FVector GetLocationInFrontOfActorCam(const AActor* Actor, const FRotator& CameraRotation, float Distance);
	
	UFUNCTION(BlueprintPure, Category="PXII|Math")
	static FVector GetLocationTopOfActor(const AActor* Actor, float Distance);

	UFUNCTION(BlueprintPure, Category="PXII|Math", meta=(WorldContext="WorldContextObject"))
	static TArray<FVector> GenerateArcPath(const UObject* WorldContextObject, const FVector& Start, const FVector& End,
		float Height = 300.f, int32 NodeCount = 20, bool bDrawDebug = false, float DebugDuration = 5.f, float drawRadius = 10.f);

	UFUNCTION(BlueprintPure, Category="PXII|Math")
	static FVector GetSafeNormal(const FVector& Vector, float Tolerance = 0.000001f) { return Vector.GetSafeNormal(Tolerance); }

	UFUNCTION(BlueprintPure, Category="PXII|Math")
	static FVector GetOffsetPositionTowardOrigin(const FVector& Origin, const FVector& Target, float Distance);

	UFUNCTION(BlueprintPure, Category="PXII|Math")
	static bool GetRandomNavigablePointInRing(UObject* WorldContextObject, const FVector& Origin, float InnerRadius, float OuterRadius,
		TSubclassOf<UNavigationQueryFilter> FilterClass, FVector& OutLocation, int32& Attempts, int32 MaxAttempts, bool bRequireReachable, bool bDrawDebug = false, float Duration = 2.f);
	
	UFUNCTION(BlueprintCallable, Category="PXII|Math")
	static bool GetRandomValidLocationBehindActor(const AActor* Actor, FVector Offset, int32 TraceCount, float DistanceBehind, float SphereRadius, float ConeAngle, FVector& OutLocation);
};
