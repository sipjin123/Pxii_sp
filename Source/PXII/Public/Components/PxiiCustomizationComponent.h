#pragma once
#include "Components/ActorComponent.h"
#include "Customization/PxiiCustomizationBaseData.h"
#include "PxiiCustomizationComponent.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiCustomizationComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetHeadSlot(UPxiiCustomizationBaseData* headData);
	
	virtual void SetHeadSlot_Implementation(UPxiiCustomizationBaseData* headData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetLowerSlot(UPxiiCustomizationBaseData* lowerData);

	virtual void SetLowerSlot_Implementation(UPxiiCustomizationBaseData* lowerData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetUpperSlot(UPxiiCustomizationBaseData* upperData);

	virtual void SetUpperSlot_Implementation(UPxiiCustomizationBaseData* upperData);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnSlotUpdated();

	virtual void OnSlotUpdated_Implementation();

	UFUNCTION(BlueprintPure)
	UPxiiCustomizationBaseData* GetCurrentHead();

	UFUNCTION(BlueprintPure)
	UPxiiCustomizationBaseData* GetCurrentUpper();

	UFUNCTION(BlueprintPure)
	UPxiiCustomizationBaseData* GetCurrentLower();

	UFUNCTION(BlueprintCallable)
	void FillCustomizationSaveData(FCustomizationSaveData& comp);

	UFUNCTION(BlueprintCallable)
	void LoadCustomization();

	UFUNCTION(BlueprintCallable)
	void LoadData(const FCustomizationSaveData& saveData);
	
private:

	UPROPERTY()
	TObjectPtr<UPxiiCustomizationBaseData> CurrentHead;

	UPROPERTY()
	TObjectPtr<UPxiiCustomizationBaseData> CurrentUpper;

	UPROPERTY()
	TObjectPtr<UPxiiCustomizationBaseData> CurrentLower;
	
};
