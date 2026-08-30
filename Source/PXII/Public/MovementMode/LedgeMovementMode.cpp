#include "LedgeMovementMode.h"
#include "DefaultMovementSet/Settings/CommonLegacyMovementSettings.h"
#include "Utility/PXIILogUtility.h"

void ULedgeMovementMode::GenerateMove_Implementation(const FMoverSimContext& SimContext,
                                                     const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep, FProposedMove& OutProposedMove) const
{
	
}

void ULedgeMovementMode::OnRegistered(const FName ModeName, const FMoverSimContext& SimContext)
{
	Super::OnRegistered(ModeName, SimContext);
}
