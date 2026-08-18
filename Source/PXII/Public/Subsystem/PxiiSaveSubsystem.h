#pragma once
#include "Components/PxiiCustomizationComponent.h"
#include "SaveData/PxiiSaveData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PxiiSaveSubsystem.generated.h"

UCLASS()
class PXII_API UPxiiSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Save")
	bool SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Save")
	bool LoadGame();

	UFUNCTION(BlueprintPure, Category = "Save")
	UPxiiSaveData* GetCurrentSave() const;

	UFUNCTION(BlueprintCallable, Category = "Save")
	bool SaveCustomization(UPxiiCustomizationComponent* comp);

	UFUNCTION(BlueprintCallable, Category = "Save")
	bool LoadCustomization(UPxiiCustomizationComponent* comp);

private:
	
	UPROPERTY()
	TObjectPtr<UPxiiSaveData> CurrentSaveData;

	UPROPERTY(EditDefaultsOnly)
	FString SaveSlot = TEXT("Player");

	UPROPERTY(EditDefaultsOnly)
	int32 UserIndex = 0;

	bool CreateSaveIfNeeded();
};
