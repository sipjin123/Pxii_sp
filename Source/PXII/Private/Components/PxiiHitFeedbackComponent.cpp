#include "Components/PxiiHitFeedbackComponent.h"

void UPxiiHitFeedbackComponent::PlayHitFlash_Implementation()
{
	HitFlashElapsed = 0.0f;

	GetWorld()->GetTimerManager().ClearTimer(
		HitFlashTimerHandle
	);

	GetWorld()->GetTimerManager().SetTimer(
		HitFlashTimerHandle,
		this,
		&UPxiiHitFeedbackComponent::UpdateHitFlash,
		0.016f,
		true
	);
}

void UPxiiHitFeedbackComponent::CreateDynamicMaterial(UMeshComponent* mesh)
{
	for (int32 index = 0; index < mesh->GetNumMaterials(); index++)
	{
		UMaterialInstanceDynamic* MID =	mesh->CreateAndSetMaterialInstanceDynamic(index);

		if (MID)
		{
			DynamicMaterials.Add(MID);
			MID->SetVectorParameterValue(TEXT("HitFlashColor"), HitFlashColor);
			MID->SetScalarParameterValue(TEXT("HitFlashAmount"),0.0f);
			MID->SetScalarParameterValue(TEXT("HitFlashIntensity"), HitFlashIntensity);
		}
	}
}

void UPxiiHitFeedbackComponent::UpdateHitFlash_Implementation()
{
	HitFlashElapsed += 0.016f;

	const float Alpha = FMath::Clamp(
		HitFlashElapsed / HitFlashDuration,
		0.0f,
		MaxOpacity
	);

	// Flash in, then fade out.
	const float FlashAmount = MaxOpacity - Alpha;

	for (UMaterialInstanceDynamic* MID : DynamicMaterials)
	{
		if (MID)
		{
			MID->SetScalarParameterValue(TEXT("HitFlashAmount"), FlashAmount);
		}
	}

	if (Alpha >= MaxOpacity)
	{
		GetWorld()->GetTimerManager().ClearTimer(HitFlashTimerHandle);
	}
}
