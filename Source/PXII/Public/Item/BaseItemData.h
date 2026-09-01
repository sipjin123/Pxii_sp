#pragma once
#include "Engine/DataAsset.h"
#include "BaseItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Invalid,
	Equipment,
};

UCLASS(BlueprintType)
class PXII_API UBaseItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly)
	FPrimaryAssetId AssetId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BuyPrice;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SellPrice;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxStackSize = 1;
	
};
