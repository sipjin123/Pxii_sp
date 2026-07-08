#pragma once
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "PxiiReticleData.generated.h"

USTRUCT(BlueprintType)
struct PXII_API FPxiiReticleData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ActiveTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Priority;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Reticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* ChargeReticle;
};
