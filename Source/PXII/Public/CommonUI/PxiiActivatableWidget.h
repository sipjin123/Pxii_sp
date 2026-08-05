#pragma once

#include "CommonActivatableWidget.h"
#include "Character/PxiiCharacter.h"
#include "PlayerController/PxiiPlayerController.h"
#include "PxiiActivatableWidget.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnActivated() override;

	virtual void NativeOnDeactivated() override;

protected:

	UFUNCTION(BlueprintPure, Category = "UI")
	APxiiPlayerController* GetPlayerController() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	APxiiCharacter* GetPlayerPawn() const;
};
