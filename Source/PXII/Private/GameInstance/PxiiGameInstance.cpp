#include "GameInstance/PxiiGameInstance.h"

#if WITH_EDITOR
#include "DataTableEditorUtils.h"
#endif
#include "Engine/AssetManager.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"
#include "LoadingScreenModule/Public/LoadingScreenModule.h"
#include "Misc/PackageName.h"
#include "LoadingScreenModule/Public/PxiiLoadingScreen.h"

TSoftObjectPtr<UWorld> UPxiiGameInstance::GetLevelSoftReferenceByTag(const FGameplayTag InLevelTag)
{
	if (LevelMapping)
	{
		if (!LevelMapping->GameLevelMapping.Contains(InLevelTag))
		{
			PXII_LOG(ELogCategory::General, Warning, TEXT("[%s]: Game level %s is not assigned in %s"), *ThisClass::StaticClass()->GetName(), *InLevelTag.ToString(), *LevelMapping->GetName());
			return nullptr;
		}
		
		return LevelMapping->GameLevelMapping.FindRef(InLevelTag);
	}
	
	return nullptr;
}

void UPxiiGameInstance::RequestLoadLevel(bool bShowLoadingScreen, TSoftObjectPtr<UWorld> LevelToLoad)
{
	if (bTransitionInProgress)
	{
		PxiiLog::Print(TEXT("Loading Screen"), FString::Printf(TEXT("RequestLoadLevel ignored - transition already in progress, %s, %s)"), (bTransitionInProgress ? TEXT("True") : TEXT("False")), *LevelToLoad.ToString()));
		return;
	}
	bTransitionInProgress = true;
	
	bShouldShowLoadingScreen = bShowLoadingScreen;
	CurrentLevel = LevelToLoad;
	
	PxiiLog::Print(ThisClass::StaticClass()->GetName(), FString::Printf(TEXT("Request Load Level")));
	
	const FString PackageName = FPackageName::ObjectPathToPackageName(LevelToLoad.ToString()); // /Game/Levels/DefaultLevel
	LoadedMapName = *FPackageName::GetShortName(PackageName); // DefaultLevel
	const FSoftObjectPath WorldPath = LevelToLoad.ToSoftObjectPath();
	
	FLoadingScreenModule* LoadingScreenModule = nullptr;
	if (bShouldShowLoadingScreen)
	{
		const FPxiiLoadingScreenTableRow* FoundDataTableRow = FindDataTableRowForMap(LoadedMapName);
	
		LoadingScreenModule = FModuleManager::LoadModulePtr<FLoadingScreenModule>("LoadingScreenModule");
		if (LoadingScreenModule)
		{
			PxiiLog::Print(TEXT("Loading Screen"), FString::Printf(TEXT("Loading Screen Module loaded")));
			LoadingScreenModule->StartLoadingScreen(FoundDataTableRow, MinimumLoadingScreenTime, LoadedMapName, MapLoadHandle);
		}
	}
	
	MapLoadHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		WorldPath,
		//FStreamableDelegate::CreateUObject(this, &UPxiiGameInstance::OnLevelLoaded),
		FStreamableDelegate(),
		FStreamableManager::AsyncLoadHighPriority
	);
	
	if (MapLoadHandle.IsValid() && LoadingScreenModule)
	{
		LoadingScreenModule->SetActiveLoadHandle(MapLoadHandle);
	}
	
	if (MapLoadHandle.IsValid())
	{
		MapLoadHandle->BindCompleteDelegate(FStreamableDelegate::CreateLambda([this]()
		{
			OnLevelLoaded();
		}));
	}
}

void UPxiiGameInstance::Init()
{
	Super::Init();

	if(CharacterVisualData)
	{
		if(!CharacterVisualData->DisableVisualOverride)
		{
			IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("DDCvar.VisualOverride"));
			if(CVar)
			{
				CVar->Set(CharacterVisualData->VisualOverrideId, ECVF_SetByCode);
			}			
		}
	}

	for (const TPair<ELogCategory, bool>& Pair : LogData->LogCategories)
	{
		ELogCategory Category = Pair.Key;
		if(Pair.Value)
		{
			PXIILogUtility::Enable(Category);
		}
		else
		{
			PXIILogUtility::Disable(Category);
		}
		// Do something
	}
	
	// For loading stuff
	// Bind Delegates
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnDestinationWorldLoaded);
}

void UPxiiGameInstance::Shutdown()
{
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	
	Super::Shutdown();
}

void UPxiiGameInstance::OnPreLoadMap(const FString& MapName)
{
	if (!bShouldShowLoadingScreen)
	{
		return;
	}
	
	// move loading to request load
}

void UPxiiGameInstance::OnDestinationWorldLoaded(UWorld* LoadedWorld)
{
	// any how will not be called now.....
	if (bShouldShowLoadingScreen)
	{
		// Called stop movie on progress bar full instead here
		// cuz the delegate will not execute for some reason and will stuck at loading screen forever

		//
		MapLoadHandle.Reset();
		bShouldShowLoadingScreen = false;
		bTransitionInProgress = false; 
		//
	}
}

const FPxiiLoadingScreenTableRow* UPxiiGameInstance::FindDataTableRowForMap(const FString& InMapName) const
{
	if (!LoadingScreenDataTable)
	{
		return nullptr;
	}
	
	static const FString Context(TEXT("PxiiLoadingScreenDataTableRowLookup"));
	
	if (bUseSingleDataTableRowForWholeGame)
	{
		return LoadingScreenDataTable->FindRow<FPxiiLoadingScreenTableRow>(SingleDataTableRowName, Context, false);
	}
	
	// Row-name-per-map convention, looks for actually matched name !!!!IMPORTANT!!!!
	const FName RowName(LoadedMapName);
	PxiiLog::Print(TEXT("Loading Screen"), FString::Printf(TEXT("Row name: %s"), *RowName.ToString()), ELogCategory::General, EPXIILogVerbosity::Warning, FColor::Red, 10);
	if (const FPxiiLoadingScreenTableRow* FoundRow = LoadingScreenDataTable->FindRow<FPxiiLoadingScreenTableRow>(RowName, Context, false))
	{
		return FoundRow;
	}
	
	return LoadingScreenDataTable->FindRow<FPxiiLoadingScreenTableRow>(DefaultDataTableRowName, Context, false);		
}

void UPxiiGameInstance::OnLevelLoaded()
{
	PxiiLog::Print(TEXT("Loading Screen"), FString::Printf(TEXT("On level loaded")));

	UGameplayStatics::OpenLevelBySoftObjectPtr(this, CurrentLevel, false, FString());
	MapLoadHandle.Reset();
}
