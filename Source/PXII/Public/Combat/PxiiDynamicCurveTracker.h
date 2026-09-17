// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PxiiCharacter.h"
#include "GameFramework/Actor.h"
#include "PxiiDynamicCurveTracker.generated.h"

UCLASS()
class PXII_API APxiiDynamicCurveTracker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APxiiDynamicCurveTracker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY()
	TObjectPtr<AActor> HomingTarget=nullptr;

	UPROPERTY()
	FVector HomingOrigin=FVector::ZeroVector;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PXII|Homing")
	float HomingDuration=1.5f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PXII|Homing")
	float CurveHeight=500.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PXII|Homing")
	bool ActiveObject;

	UPROPERTY()
	float HomingElapsedTime=0.f;

	UFUNCTION(BlueprintCallable)
	void ActivateObject();
	
	UFUNCTION(BlueprintCallable)
	void InitializeHoming(AActor* Target,float Duration,float InCurveHeight);
	FVector CalculateHomingCurve(float Alpha) const;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PXII|Homing")
	float CurveAngle=45.f;
	
	UPROPERTY()
	FVector CurveDirection=FVector::ZeroVector;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FHitTarget HitTarget;
};
