#pragma once
#include "Engine/DataAsset.h"
#include "PxiiCustomizationBaseData.generated.h"

UCLASS(BlueprintType)
class PXII_API UPxiiCustomizationBaseData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemKey;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemDescription;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsLocked = false;
};
