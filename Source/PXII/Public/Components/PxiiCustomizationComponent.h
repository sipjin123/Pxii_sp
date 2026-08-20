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
	void SetHeadSlot(UPxiiCustomizationBaseData* headData, bool saveChange = false);
	
	virtual void SetHeadSlot_Implementation(UPxiiCustomizationBaseData* headData, bool saveChange = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetLowerSlot(UPxiiCustomizationBaseData* lowerData, bool saveChange = false);

	virtual void SetLowerSlot_Implementation(UPxiiCustomizationBaseData* lowerData, bool saveChange = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetUpperSlot(UPxiiCustomizationBaseData* upperData, bool saveChange = false);

	virtual void SetUpperSlot_Implementation(UPxiiCustomizationBaseData* upperData, bool saveChange = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetTattooSlot(UPxiiCustomizationBaseData* tattooData, bool saveChange = false);

	virtual void SetTattooSlot_Implementation(UPxiiCustomizationBaseData* tattooData, bool saveChange = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetBlasterSlot(UPxiiCustomizationBaseData* blasterData, bool saveChange = false);

	virtual void SetBlasterSlot_Implementation(UPxiiCustomizationBaseData* blasterData, bool saveChange = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnSlotUpdated(bool saveChange = false);

	virtual void OnSlotUpdated_Implementation(bool saveChange = false);

	UFUNCTION(BlueprintPure)
	UPxiiCustomizationBaseData* GetCurrentHead();

	UFUNCTION(BlueprintPure)
	UPxiiCustomizationBaseData* GetCurrentUpper();

	UFUNCTION(BlueprintPure)
	UPxiiCustomizationBaseData* GetCurrentLower();

	UFUNCTION(BlueprintPure)
	UPxiiCustomizationBaseData* GetCurrentBlaster();

	UFUNCTION(BlueprintPure)
	UPxiiCustomizationBaseData* GetCurrentTattoo();

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

	UPROPERTY()
	TObjectPtr<UPxiiCustomizationBaseData> CurrentTattoo;

	UPROPERTY()
	TObjectPtr<UPxiiCustomizationBaseData> CurrentBlaster;
	
};
