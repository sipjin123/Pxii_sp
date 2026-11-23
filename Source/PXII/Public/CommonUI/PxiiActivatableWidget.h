#pragma once

#include "CommonActivatableWidget.h"
#include "Character/PxiiCharacter.h"
#include "PlayerController/PxiiPlayerController.h"
#include "PxiiActivatableWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE(FInputActionExecutedDelegate);
DECLARE_DYNAMIC_DELEGATE(FInputActionHoldStartedDelegate);
DECLARE_DYNAMIC_DELEGATE_OneParam(FInputActionHoldProgressDelegate, float, HoldProgress);
DECLARE_DYNAMIC_DELEGATE(FInputActionHoldReleasedDelegate);

USTRUCT(BlueprintType)
struct FInputActionBindingHandle
{
	GENERATED_BODY()
	
public:
	FUIActionBindingHandle Handle;
};

UCLASS(Blueprintable)
class PXII_API UPxiiActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION(BlueprintCallable, Category = "PXII|UI")
	void RegisterBinding(FDataTableRowHandle InputAction, const bool bShowInActionBar, 
		FInputActionExecutedDelegate NormalCallback, 
		FInputActionHoldStartedDelegate OnHoldStarted,
		FInputActionHoldProgressDelegate OnHoldProgress,
		FInputActionHoldReleasedDelegate OnHoldReleased,
		FInputActionBindingHandle& BindingHandle
	);

	UFUNCTION(BlueprintCallable, Category = "PXII|UI")
	void UnregisterBinding(FInputActionBindingHandle BindingHandle);
	
	UFUNCTION(BlueprintCallable, Category = "PXII|UI")
	void UnregisterAllBindings();
	
protected:

	UFUNCTION(BlueprintPure, Category = "PXII|UI")
	APxiiPlayerController* GetPlayerController() const;

	UFUNCTION(BlueprintPure, Category = "PXII|UI")
	APxiiCharacter* GetPlayerPawn() const;
	
private:
	TArray<FUIActionBindingHandle> BindingHandles;
};
