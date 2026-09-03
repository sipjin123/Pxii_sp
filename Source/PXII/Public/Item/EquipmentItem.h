#pragma once
#include "BaseItem.h"
#include "EquipmentItemData.h"
#include "EquipmentItem.generated.h"

UCLASS()
class UEquipmentItem : public UBaseItem
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(FItemSaveData saveData) override;
	virtual void Initialize(FPrimaryAssetId assetId) override;

	virtual UBaseItemData* GetData() override;

	UFUNCTION(BlueprintPure)
	virtual UEquipmentItemData* GetEquipmentData();

	virtual FItemSaveData GetSaveData() override;
	
private:

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UEquipmentItemData> EquipmentItemData;
	
};
