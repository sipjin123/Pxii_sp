// Fill out your copyright notice in the Description page of Project Settings.


#include "Structs/PxiiLayeredMovePath.h"

FLayeredMoveBase* FLayeredMove_Path::Clone() const
{
	return new FLayeredMove_Path(*this);
}
bool FLayeredMove_Path::GenerateMove(const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep,
                                     const UMoverComponent* MoverComp, UMoverBlackboard* SimBlackboard, FProposedMove& OutProposedMove)
{
	
	UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: SHEEEEEEEEEEEEEEEEEEEEEET By"));
	return FLayeredMoveBase::GenerateMove(StartState, TimeStep, MoverComp, SimBlackboard, OutProposedMove);
}
