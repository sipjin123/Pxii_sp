#pragma once
#include "CommonUI/PxiiActivatableWidget.h"
#include "PxiiHUD.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiHUD : public UPxiiActivatableWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleADS(bool onADS);
};
