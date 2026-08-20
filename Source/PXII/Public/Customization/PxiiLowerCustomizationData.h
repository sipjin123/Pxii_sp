#pragma once
#include "PxiiCustomizationBaseData.h"
#include "Materials/MaterialInterface.h"
#include "PxiiLowerCustomizationData.generated.h"

UCLASS(BlueprintType)
class PXII_API UPxiiLowerCustomizationData : public UPxiiCustomizationBaseData
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FString, TObjectPtr<UMaterialInterface>> LowerMaterialMap;
};
