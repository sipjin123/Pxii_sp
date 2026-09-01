#pragma once
#include "BaseItemData.h"
#include "SaveData/PxiiSaveData.h"
#include "BaseItem.generated.h"

UCLASS()
class PXII_API UBaseItem : public UObject
{
	GENERATED_BODY()
	
	
public:
	
	virtual void Initialize(FPrimaryAssetId assetId);
	virtual void Initialize(FItemSaveData saveData);
	
	FGuid GetInstanceId();

	virtual UBaseItemData* GetData();

	int32 GetItemQuantity();

	virtual FItemSaveData GetSaveData();

private:
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FGuid InstanceId;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UBaseItemData> BaseItemData;

	int32 Quantity = 0;
	
	UBaseItemData* GetItemData(FPrimaryAssetId assetId);

};
