#pragma once
#include "Engine/DataAsset.h"
#include "PxiiYinYangData.generated.h"

UENUM(BlueprintType)
enum class EWeaponResourceType : uint8
{
	Invalid,
	Yin,
	Yang,
};

UCLASS(BlueprintType)
class PXII_API UResourceGenerationCost : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponResourceType CostType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CostValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponResourceType GenerateType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float GenerateValue;

	bool IsValid()
	{
		if(CostType == EWeaponResourceType::Invalid || GenerateType == EWeaponResourceType::Invalid)
		{
			return false;
		}
		
		return CostType != GenerateType;
	}
	
};

UCLASS(BlueprintType)
class PXII_API UPxiiYinYangData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Yin = 50.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Yang = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UResourceGenerationCost> DefaultRangeCost;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UResourceGenerationCost> DefaultMeleeCost;
	
};
