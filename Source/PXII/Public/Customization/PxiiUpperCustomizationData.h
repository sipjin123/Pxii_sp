#pragma once
#include "PxiiCustomizationBaseData.h"
#include "Materials/MaterialInterface.h"
#include "PxiiUpperCustomizationData.generated.h"

UCLASS(BlueprintType)
class PXII_API UPxiiUpperCustomizationData : public UPxiiCustomizationBaseData
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FString, TObjectPtr<UMaterialInterface>> UpperMaterialMap;
};
