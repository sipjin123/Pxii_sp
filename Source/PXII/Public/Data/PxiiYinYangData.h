#pragma once
#include "GameplayEffect.h"
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

	bool IsValid()
	{
		if(CostType == EWeaponResourceType::Invalid)
		{
			return false;
		}
		
		return true;
	}
	
};

UCLASS(BlueprintType)
class PXII_API UPxiiYinYangData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float YinStress;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> YinStressGE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float YinOverwhelm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> YinOverwhelmGE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float YinFull;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> YinFullGE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float YangStress;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> YangStressGE;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float YangOverwhelm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> YangOverwhelmGE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float YangFull;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> YangFullGE;
	
};
