// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/PxiiPropsData.h"
#include "PxiiPropsBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDestructState, bool, IsDestroyed, float, Magnitude);

UCLASS()
class PXII_API APxiiPropsBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APxiiPropsBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite)
	EPropsType PropType;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentPropLife = 10;
	UPROPERTY(BlueprintReadWrite)
	int32 MaxPropLife = 10;

	UPROPERTY(BlueprintReadWrite)
	bool IsDestroyed;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FDestructState DestructState;
};
