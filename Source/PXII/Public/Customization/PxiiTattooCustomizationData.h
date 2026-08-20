#pragma once
#include "PxiiCustomizationBaseData.h"
#include "Materials/MaterialInterface.h"
#include "PxiiTattooCustomizationData.generated.h"

UCLASS(BlueprintType)
class PXII_API UPxiiTattooCustomizationData : public UPxiiCustomizationBaseData
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> TattooMaterial;
};
