#pragma once
#include "Customization/PxiiCustomizationHandler.h"
#include "Customization/PxiiCustomizationMap.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CharacterCustomizationSubsystem.generated.h"

UCLASS(BlueprintType)
class UCharacterCustomizationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintPure)
	FString GetHeadSlotKey();
	
	UFUNCTION(BlueprintPure)
	FString GetUpperSlotKey();

	UFUNCTION(BlueprintPure)
	FString GetLowerSlotKey();

	UFUNCTION(BlueprintPure)
	FString GetTattooSlotKey();

	UFUNCTION(BlueprintPure)
	FString GetBlasterSlotKey();

	UFUNCTION(BlueprintPure)
	FString GetHeadSecondarySlotKey();
	
	UFUNCTION(BlueprintPure)
	FString GetUpperSecondarySlotKey();

	UFUNCTION(BlueprintPure)
	FString GetLowerSecondarySlotKey();

	UFUNCTION(BlueprintPure)
	FString GetTattooSecondarySlotKey();

	UFUNCTION(BlueprintPure)
	FString GetBlasterSecondarySlotKey();

	UFUNCTION(BlueprintPure)
	TArray<UPxiiCustomizationBaseData*> GetHeadData();

	UFUNCTION(BlueprintPure)
	TArray<UPxiiCustomizationBaseData*> GetUpperData();

	UFUNCTION(BlueprintPure)
	TArray<UPxiiCustomizationBaseData*> GetLowerData();

	UFUNCTION(BlueprintPure)
	TArray<UPxiiCustomizationBaseData*> GetTattooData();
	
	UFUNCTION(BlueprintPure)
	TArray<UPxiiCustomizationBaseData*> GetBlasterData();

	UFUNCTION(BlueprintCallable)
	void Initialize(UPxiiCustomizationMap* InCustomizationMap);

	UFUNCTION(BlueprintCallable)
	void SetCustomizationHandler(APxiiCustomizationHandler* InCustomizationMap);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ProcessCustomizationMap();
	
	void ProcessCustomizationMap_Implementation();

	UFUNCTION(BlueprintCallable)
	void OpenCustomizationLevel(bool closeIfActive = false);

	UFUNCTION(BlueprintCallable)
	void CloseCustomizationLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool FindHeadData(UPxiiCustomizationBaseData*& headData, const FString& itemKey);

	bool FindHeadData_Implementation(UPxiiCustomizationBaseData*& headData, const FString& itemKey);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool FindUpperData(UPxiiCustomizationBaseData*& UpperData, const FString& itemKey);
	
	bool FindUpperData_Implementation(UPxiiCustomizationBaseData*& UpperData, const FString& itemKey);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool FindLowerData(UPxiiCustomizationBaseData*& LowerData, const FString& itemKey);

	bool FindLowerData_Implementation(UPxiiCustomizationBaseData*& LowerData, const FString& itemKey);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool FindTattooData(UPxiiCustomizationBaseData*& TattooData, const FString& itemKey);

	bool FindTattooData_Implementation(UPxiiCustomizationBaseData*& TattooData, const FString& itemKey);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool FindBlasterData(UPxiiCustomizationBaseData*& BlasterData, const FString& itemKey);

	bool FindBlasterData_Implementation(UPxiiCustomizationBaseData*& BlasterData, const FString& itemKey);

	UFUNCTION(BlueprintPure)
	APxiiCustomizationHandler* GetCustomizationHandler();
	
private:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPxiiCustomizationMap> CustomizationMap;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<APxiiCustomizationHandler> CustomizationHandler;
};
