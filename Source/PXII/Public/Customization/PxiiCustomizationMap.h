#pragma once
#include "PxiiCustomizationSlot.h"
#include "Engine/DataAsset.h"
#include "PxiiCustomizationMap.generated.h"

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	Invalid,
	Head,
	Upper,
	Lower,
	Tattoo,
	Blaster
};

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPxiiCustomizationSlot> TattooSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPxiiCustomizationSlot> BlasterSlot;
};
