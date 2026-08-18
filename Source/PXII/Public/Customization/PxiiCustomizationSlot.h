#pragma once
#include "PxiiCustomizationBaseData.h"
#include "Engine/DataAsset.h"
#include "PxiiCustomizationSlot.generated.h"

UCLASS(BlueprintType)
class UPxiiCustomizationSlot : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString SlotKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString SecondarySlotKey;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UPxiiCustomizationBaseData>> ItemData;
};
