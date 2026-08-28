#pragma once
#include "MovementMode.h"
#include "DefaultMovementSet/Modes/SmoothWalkingMode.h"
#include "LedgeMovementMode.generated.h"

UCLASS(Blueprintable)
class PXII_API ULedgeMovementMode : public UBaseMovementMode
{
	GENERATED_BODY()
	
public:

	virtual void GenerateMove_Implementation(const FMoverSimContext& SimContext, const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep, FProposedMove& OutProposedMove) const override;
	virtual void OnRegistered(const FName ModeName, const FMoverSimContext& SimContext) override;

private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float ShimmySpeed = 130.0f;
	
};
