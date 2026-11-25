#include "Components/PxiiInteractableComponent.h"

#include "Utility/PXIILogUtility.h"

void UPxiiInteractableComponent::SetInteractable_Implementation(APxiiInteractableBase* interactableItem)
{
	PXII_LOG(ELogCategory::Projectile, Log, TEXT("DZ_LOG:: Interactable is SET"));
	Interactable = interactableItem;
	if(Interactable == nullptr)
	{
		PXII_LOG(ELogCategory::Projectile, Log, TEXT("DZ_LOG:: Interactable is NULL"));
	}
	
}

void UPxiiInteractableComponent::OnInteractionStarted_Implementation()
{
	if(Interactable == nullptr)
	{
		PXII_LOG(ELogCategory::Projectile, Log, TEXT("DZ_LOG:: Interactable is NULL"));
		return;
	}

	if(!Interactable->IsInteractable())
	{
		PXII_LOG(ELogCategory::Projectile, Log, TEXT("DZ_LOG:: Interactable is NOT interactable"));
		return;
	}

	PXII_LOG(ELogCategory::Projectile, Log, TEXT("DZ_LOG:: Interactable Success"));
	Interactable->InteractionStarted();
}

void UPxiiInteractableComponent::OnInteractionReleased_Implementation()
{
	Interactable->InteractionReleased();
}
