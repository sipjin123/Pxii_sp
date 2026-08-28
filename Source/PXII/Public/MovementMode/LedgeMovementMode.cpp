#include "LedgeMovementMode.h"
#include "DefaultMovementSet/Settings/CommonLegacyMovementSettings.h"
#include "Utility/PXIILogUtility.h"

void ULedgeMovementMode::GenerateMove_Implementation(const FMoverSimContext& SimContext,
                                                     const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep, FProposedMove& OutProposedMove) const
{

	OutProposedMove.LinearVelocity = FVector(0, 600, 0);
	PXII_LOG(ELogCategory::General,	Log, TEXT("Proposed Velocity: %s"),	*OutProposedMove.LinearVelocity.ToString());
	Super::GenerateMove_Implementation(SimContext, StartState, TimeStep, OutProposedMove);
}

void ULedgeMovementMode::OnRegistered(const FName ModeName, const FMoverSimContext& SimContext)
{
	Super::OnRegistered(ModeName, SimContext);
	// if (UMoverComponent* MoverComp = GetMoverComponent())
	// {
	// 	CommonLegacySettings = MoverComp->FindSharedSettings<UCommonLegacyMovementSettings>();
	// 	OriginalCapsuleHalfHeight = MoverComp->GetOwner()->GetSimpleCollisionHalfHeight();
	// }
	// ensureMsgf(CommonLegacySettings, TEXT("Failed to find instance of CommonLegacyMovementSettings on %s. Movement may not function properly."), *GetPathNameSafe(this));
}
