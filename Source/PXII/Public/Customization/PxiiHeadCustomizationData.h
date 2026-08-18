#pragma once
#include "PxiiCustomizationBaseData.h"
#include "GroomAsset.h"
#include "PxiiHeadCustomizationData.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiHeadCustomizationData : public UPxiiCustomizationBaseData
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UGroomAsset> Groom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UGroomBindingAsset> GroomBinding;
};
