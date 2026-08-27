// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PxiiCharacter.h"
#include "GameFramework/Actor.h"
#include "PxiiArcObject.generated.h"

UCLASS()
class PXII_API APxiiArcObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APxiiArcObject();

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void InitializeArc(const TArray<FVector>& InArcPoints, float InTravelTime, bool bDormantState);

	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> ArcPoints;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FHitTarget HitTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bUseSweep;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsDormant;
protected:
	virtual void BeginPlay() override;

private:

	int32 CurrentIndex = 0;
	float TravelTime = 1.0f;
	float ElapsedTime = 0.0f;
	float TimePerSegment = 0.0f;

	FVector StartPos;
	FVector EndPos;
};
