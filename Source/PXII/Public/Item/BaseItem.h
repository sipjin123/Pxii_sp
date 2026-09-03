#pragma once
#include "BaseItemData.h"
#include "SaveData/PxiiSaveData.h"
#include "BaseItem.generated.h"

UCLASS(BlueprintType)
class PXII_API UBaseItem : public UObject
{
	GENERATED_BODY()
	
	
public:
	
	virtual void Initialize(FPrimaryAssetId assetId);
	virtual void Initialize(FItemSaveData saveData);

	UFUNCTION(BlueprintPure)
	FGuid GetInstanceId();

	UFUNCTION(BlueprintPure)
	virtual UBaseItemData* GetData();

	UFUNCTION(BlueprintPure)
	int32 GetItemQuantity() const;

	UFUNCTION(BlueprintCallable)
	void AddStack();

	virtual FItemSaveData GetSaveData();

	bool CanStack() const;

protected:
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	FGuid InstanceId;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UBaseItemData> BaseItemData;

	int32 Quantity = 0;
	
	UBaseItemData* GetItemData(FPrimaryAssetId assetId);

};
