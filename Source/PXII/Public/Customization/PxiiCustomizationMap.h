#pragma once
#include "PxiiCustomizationSlot.h"
#include "Engine/DataAsset.h"
#include "PxiiCustomizationMap.generated.h"

UCLASS(BlueprintType)
class PXII_API UPxiiCustomizationMap : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPxiiCustomizationSlot> HeadSlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPxiiCustomizationSlot> UpperBodySlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPxiiCustomizationSlot> LowerBodySlot;
};
