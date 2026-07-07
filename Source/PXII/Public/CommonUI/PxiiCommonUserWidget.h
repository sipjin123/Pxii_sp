#pragma once
#include "CommonUserWidget.h"
#include "Character/PxiiCharacter.h"
#include "PlayerController/PxiiPlayerController.h"
#include "PxiiCommonUserWidget.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiCommonUserWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintPure, Category = "UI")
	APxiiPlayerController* GetPlayerController() const;

	UFUNCTION(BlueprintPure, Category = "UI")
	APxiiCharacter* GetPlayerPawn() const;
};