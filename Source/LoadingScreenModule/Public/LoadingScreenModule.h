#pragma once

#include "CoreMinimal.h"
#include "PxiiLoadingScreen.h"
#include "PxiiLoadingScreenTableRow.h"
#include "Engine/StreamableManager.h"
#include "Modules/ModuleInterface.h"

class LOADINGSCREENMODULE_API FLoadingScreenModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    virtual bool IsGameModule() const override;
    virtual void StartLoadingScreen(const FPxiiLoadingScreenTableRow* FoundDataTableRow, float MinimumLoadingScreenTime, FString LoadedMapName, TSharedPtr<FStreamableHandle> MapLoadHandle);
    virtual void EndLoadingScreen();
    void SetActiveLoadHandle(TSharedPtr<FStreamableHandle> Handle);
    
private:
    TSharedPtr<SPxiiLoadingScreen> ActiveLoadingScreenWidget;
    
#pragma region Engine Tick Issues
    int32 InitialRayTracingValue = 1;
#pragma endregion 
};
