// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LayeredMoveBase.h"
#include "UObject/Object.h"
#include "PxiiLayeredMovePath.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FLayeredMove_Path : public FLayeredMoveBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> Path;

	UPROPERTY(BlueprintReadWrite)
	float Speed = 600.f;

	int32 CurrentIndex = 0;
	
	virtual FLayeredMoveBase* Clone() const override;

	virtual bool GenerateMove(
		const FMoverTickStartData& StartState,
		const FMoverTimeStep& TimeStep,
		const UMoverComponent* MoverComp,
		UMoverBlackboard* SimBlackboard,
		FProposedMove& OutProposedMove) override;
};