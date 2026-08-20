#pragma once
#include "Engine/DataAsset.h"
#include "PXIICharacterVisualData.generated.h"

UCLASS(Blueprintable)
class PXII_API UPXIICharacterVisualData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 VisualOverrideId;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool DisableVisualOverride = false;
};
