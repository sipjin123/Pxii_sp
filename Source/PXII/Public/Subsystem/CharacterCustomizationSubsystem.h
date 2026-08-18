#pragma once
#include "Customization/PxiiCustomizationMap.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CharacterCustomizationSubsystem.generated.h"

UCLASS(Blueprintable)
class UCharacterCustomizationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	FString GetHeadSlotKey();
	FString GetUpperSlotKey();
	FString GetLowerSlotKey();

	UFUNCTION(BlueprintPure)
	TArray<UPxiiCustomizationBaseData*> GetHeadData();

	UFUNCTION(BlueprintPure)
	TArray<UPxiiCustomizationBaseData*> GetUpperData();

	UFUNCTION(BlueprintPure)
	TArray<UPxiiCustomizationBaseData*> GetLowerData();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ProcessCustomizationMap();
	
	void ProcessCustomizationMap_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool FindHeadData(UPxiiCustomizationBaseData*& headData, const FString& itemKey);

	bool FindHeadData_Implementation(UPxiiCustomizationBaseData*& headData, const FString& itemKey);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool FindUpperData(UPxiiCustomizationBaseData*& UpperData, const FString& itemKey);
	
	bool FindUpperData_Implementation(UPxiiCustomizationBaseData*& UpperData, const FString& itemKey);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool FindLowerData(UPxiiCustomizationBaseData*& LowerData, const FString& itemKey);

	bool FindLowerData_Implementation(UPxiiCustomizationBaseData*& LowerData, const FString& itemKey);

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPxiiCustomizationMap> CustomizationMap;
	
};
