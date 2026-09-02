// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enum/PxiiDamageType.h"
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
	static FVector GetLocationRightOfActor(const AActor* Actor, float Distance);

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
	static bool GetRandomValidLocationActorDirection(const AActor* Actor, FVector Offset, int32 TraceCount, float DistanceBehind, float SphereRadius, float ConeAngle, FVector& OutLocation, EPxiiDirection PxiiDirection = EPxiiDirection::Back, float DebugDuration = .5f);

	UFUNCTION(BlueprintPure, Category="Combat|Weapons")
	static TArray<FVector> GenerateShotgunTrajectories(FVector Origin, FVector ForwardDirection, float Distance, int32 PelletCount, float SpreadAngleDegrees);

	UFUNCTION(BlueprintPure, Category="PXII|Math")
	static TArray<FVector> GenerateRandomPointsInSphere(FVector Origin, FVector ForwardDirection, float Radius, float Flatness, int32 NumPoints, float MinSpacing);
	
	UFUNCTION(BlueprintPure, Category="PXII|Math")
	static TArray<FVector> GenerateRandomPointsInRing(FVector Origin, FVector ForwardDirection, float InnerRadius, float OuterRadius, int32 NumPoints, float MinSpacing);

	UFUNCTION(BlueprintCallable, Category="Utilities|Curves", meta=(WorldContext="WorldContextObject"))
	static void GenerateSimpleArc(const bool bDrawDebug, const bool bUseRandom,
		const UObject* WorldContextObject, const FVector& Start, const FVector& End,
		float Height, int32 NumPoints, TArray<FVector>& OutPoints, FVector ArcDirection);

	UFUNCTION(BlueprintPure,Category="PXII|Math")
	static TArray<FVector> GenerateParabolicPath(FVector Origin, FVector TargetLocation, int32 NodeCount, float ParabolicHeight);

	UFUNCTION(BlueprintPure,Category="PXII|Projectile")
	static bool CalculateProjectileVelocity(FVector Start,FVector Target,float ApexHeight,float Gravity,FVector& OutVelocity);
	
	UFUNCTION(BlueprintPure,Category="PXII|Combat")
	static bool IsActorBehindContext(AActor* SourceActor,AActor* ContextActor);
	
	UFUNCTION(BlueprintPure,Category="PXII|Combat")
	static bool IsActorInFrontOfContext(AActor* SourceActor,AActor* ContextActor);

	UFUNCTION(BlueprintPure,Category="PXII|Math")
	static FVector GetPointFromTargetTowardSource(FVector Source,FVector Target,float DistanceFromTarget);
	UFUNCTION(BlueprintPure,Category="PXII|AI")
	static TArray<FVector> GenerateStrafeNodes(AActor* SourceActor,AActor* TargetActor,int32 NodeCount,float Radius,FVector& OutInitialTargetLocation);

	UFUNCTION(BlueprintPure,Category="PXII|Math")
	static TArray<int32> GetUniqueRandomIndices(int32 TotalNumber,int32 TargetNumber);
};
