#pragma once
#include "PxiiCustomizationBaseData.h"
#include "Materials/MaterialInterface.h"
#include "PxiiBlasterCustomizationData.generated.h"

UCLASS(BlueprintType)
class PXII_API UPxiiBlasterCustomizationData : public UPxiiCustomizationBaseData
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FString, TObjectPtr<UMaterialInterface>> BlasterMaterialMap;
};
