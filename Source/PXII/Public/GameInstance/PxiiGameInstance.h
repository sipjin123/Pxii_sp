#pragma once
#include "Data/PXIICharacterVisualData.h"
#include "Data/PxiiGameLevelMapping.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "Utility/PXIILogUtility.h"
#include "Log/PxiiLogData.h"
#include "Runtime/Engine/Classes/Engine/GameInstance.h"
#include "LoadingScreenModule/Public/PxiiLoadingScreenTableRow.h"
#include "PxiiGameInstance.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPXIICharacterVisualData> CharacterVisualData;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPxiiLogData> LogData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPxiiGameLevelMapping> LevelMapping;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game | Loading Screen")
	float MinimumLoadingScreenTime = 2.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Game | Loading Screen")
	TObjectPtr<UDataTable> LoadingScreenDataTable;
	
	UPROPERTY(EditDefaultsOnly, Category = "Game | Loading Screen")
	FName DefaultDataTableRowName = "Default";
	
	UPROPERTY(EditDefaultsOnly, Category = "Game | Loading Screen")
	bool bUseSingleDataTableRowForWholeGame = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Game | Loading Screen", meta = (EditCondition = "bUseSingleDataTableRowForWholeGame"))
	FName SingleDataTableRowName = "Default";

	UFUNCTION(BlueprintPure, Category = "Game")
	TSoftObjectPtr<UWorld> GetLevelSoftReferenceByTag(UPARAM(meta = (Categories = "Pxii.Level")) const FGameplayTag InLevelTag);
	
	UFUNCTION(BlueprintCallable, Category = "Game | Loading Screen")
	void RequestLoadLevel(bool bShowLoadingScreen, TSoftObjectPtr<UWorld> LevelToLoad);
	
protected:
	TSharedPtr<FStreamableHandle> MapLoadHandle;
	bool bShouldShowLoadingScreen = false;
	bool bTransitionInProgress = false;
	FString LoadedMapName;
	
	TSoftObjectPtr<UWorld> CurrentLevel;
	
	//~ Begin UGameInstance Interface
	virtual void Init() override;
	virtual void Shutdown() override;
	//~ End UGameInstance Interface

#pragma region Loading Screen
	virtual void OnPreLoadMap(const FString& MapName);
	virtual void OnDestinationWorldLoaded(UWorld* LoadedWorld);
#pragma endregion 
	
	const FPxiiLoadingScreenTableRow* FindDataTableRowForMap(const FString& InMapName) const;
	
	void OnLevelLoaded();
};
