#pragma once
#include "CommonUI/PxiiActivatableWidget.h"
#include "PxiiCustomizationUIBase.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiCustomizationUIBase : public UPxiiActivatableWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintNativeEvent)
	void OnBackBoundActionTriggered();
	void OnBackBoundActionTriggered_Implementation();

};
