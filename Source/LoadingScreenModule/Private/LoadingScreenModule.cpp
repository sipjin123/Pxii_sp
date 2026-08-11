#include "LoadingScreenModule.h"
#include "MoviePlayer.h"
#include "PxiiLoadingScreen.h"
#include "PxiiLoadingScreenLogUtility.h"
#include "PxiiLoadingScreenStyle.h"

#define LOCTEXT_NAMESPACE "FLoadingScreenModule"

void FLoadingScreenModule::StartupModule()
{
	FPxiiLoadingScreenStyle::Initialize();
}

void FLoadingScreenModule::ShutdownModule()
{
	FPxiiLoadingScreenStyle::Shutdown();
}

bool FLoadingScreenModule::IsGameModule() const
{
	return true;
}

void FLoadingScreenModule::StartLoadingScreen(const FPxiiLoadingScreenTableRow* FoundDataTableRow, float MinimumLoadingScreenTime, FString LoadedMapName, TSharedPtr<FStreamableHandle> MapLoadHandle)
{
	PxiiLoadingScreenLog::Print(TEXT("[Loading Screen] "), FString::Printf(TEXT("Currently loading: %s"), *LoadedMapName));
	
	if (GetMoviePlayer()->IsMovieCurrentlyPlaying())
	{
		return;
	}
	
	// Open loading screen
	FLoadingScreenAttributes LoadingScreenAttributes;
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = false; // Suspected here is the 1 frame
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = FoundDataTableRow ? FoundDataTableRow->MinimumDisplayTime : MinimumLoadingScreenTime;
	LoadingScreenAttributes.bWaitForManualStop = true;
	LoadingScreenAttributes.bAllowEngineTick = false;

	TSharedRef<SPxiiLoadingScreen> LoadingScreenWidget = SNew(SPxiiLoadingScreen)
		.DataRow(FoundDataTableRow) 
		.LoadedMapName(FName(LoadedMapName))
		.MapLoadHandle(MapLoadHandle);
		
	ActiveLoadingScreenWidget = LoadingScreenWidget;
	LoadingScreenAttributes.WidgetLoadingScreen = LoadingScreenWidget;
	
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}

void FLoadingScreenModule::EndLoadingScreen()
{
	PxiiLoadingScreenLog::Print(TEXT("[Loading Screen] "), FString::Printf(TEXT("Map loaded!")));
	// Close loading screen
	if (GetMoviePlayer()->IsMovieCurrentlyPlaying())
	{
		PxiiLoadingScreenLog::Print(TEXT("[Loading Screen] "), FString::Printf(TEXT("is movie currently playing")));
		ActiveLoadingScreenWidget->StartFadeOut();
	}
}

void FLoadingScreenModule::SetActiveLoadHandle(TSharedPtr<FStreamableHandle> Handle)
{
	if (ActiveLoadingScreenWidget.IsValid())
	{
		ActiveLoadingScreenWidget->SetLoadHandle(Handle);
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_GAME_MODULE(FLoadingScreenModule, LoadingScreenModule);