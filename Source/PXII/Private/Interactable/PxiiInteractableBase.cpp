#include "Interactable/PxiiInteractableBase.h"

#include "Utility/PXIILogUtility.h"

void APxiiInteractableBase::InteractionStarted_Implementation()
{
	PXII_LOG(ELogCategory::General, Log, TEXT("DZ_LOG:: INTERACTION STARTED"));
}

void APxiiInteractableBase::InteractionReleased_Implementation()
{
	PXII_LOG(ELogCategory::General, Log, TEXT("DZ_LOG:: INTERACTION CANCELLED"));
}

void APxiiInteractableBase::InteractionCompleted_Implementation()
{
	ToggleInteractable(false);
	if(!AutoResetOnComplete)
	{
		return;
	}

	StartResetTimer();
}

void APxiiInteractableBase::ResetInteractable_Implementation()
{
	ToggleInteractable(true);
}

void APxiiInteractableBase::StartResetTimer_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(ReactivationTimeHandler);
	GetWorld()->GetTimerManager().SetTimer(
		ReactivationTimeHandler,
		this,
		&APxiiInteractableBase::ResetInteractable,
		ReactivationTime,
		false
	);
}

void APxiiInteractableBase::ToggleInteractable_Implementation(bool isEnable)
{
	IsInteractableActive = isEnable;
}

bool APxiiInteractableBase::IsInteractable()
{
	return IsInteractableActive;
}
