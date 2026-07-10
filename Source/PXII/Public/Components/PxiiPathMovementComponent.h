// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LayeredMoveBase.h"
#include "Components/ActorComponent.h"
#include "PxiiPathMovementComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class PXII_API UPxiiPathMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPxiiPathMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Path")
	void StartPath(const TArray<FVector>& InPath, float InDuration);

	UFUNCTION(BlueprintCallable, Category="Path")
	void StopPath();

	UFUNCTION(BlueprintPure, Category="Path")
	bool IsMoving() const { return bMoving; }

	UFUNCTION(BlueprintPure, Category="Path")
	FVector GetCurrentLocation() const { return CurrentLocation; }

	UFUNCTION(BlueprintPure, Category="Path")
	float GetProgress() const { return Progress; }

	UFUNCTION(BlueprintCallable)
	int32 GetIndex() { return CurrIndex; }
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> Path;
protected:

	virtual void BeginPlay() override;

private:
	FVector CurrentLocation = FVector::ZeroVector;

	int32 CurrIndex;
	float Duration = 1.f;
	float Elapsed = 0.f;
	float Progress = 0.f;
	bool bMoving = false;

	
	//---------------------------------------
	
	//TArray<FVector> Path;
	TArray<float> CumulativeDistance;

	float MoveSpeed = 1200.f;
	float ElapsedDistance = 0.f;
	float TotalDistance = 0.f;

	//int32 CurrIndex = 0;

	//bool bMoving = false;

	//FVector CurrentLocation;
};