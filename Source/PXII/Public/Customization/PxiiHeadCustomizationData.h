#pragma once
#include "PxiiCustomizationBaseData.h"
#include "GroomAsset.h"
#include "PxiiHeadCustomizationData.generated.h"

UENUM(BlueprintType)
enum class EGroomType : uint8
{
	Beard,
	Eyebrows,
	Eyelashes,
	Fuzz,
	Hair,
	Mustache,
};

USTRUCT(BlueprintType)
struct FGroomItemData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UGroomAsset> Groom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UGroomBindingAsset> GroomBinding;
};

UCLASS(Blueprintable)
class PXII_API UPxiiHeadCustomizationData : public UPxiiCustomizationBaseData
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EGroomType, FGroomItemData> GroomMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> Mesh;
};
