#pragma once
#include "BaseItemData.h"
#include "GameplayTagContainer.h"
#include "EquipmentItemData.generated.h"

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	Invalid,
	Blaster,
	ArmBlade,
	Head,
	Upper,
	Lower,
	Trinket,
};

UCLASS()
class PXII_API UEquipmentItemData : public UBaseItemData
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipmentSlot EquipmentSlot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer GrantedTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	TSoftClassPtr<AActor> EquipmentClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsEquipped = false;
};
