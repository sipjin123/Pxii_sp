// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Subsystem/PxiiUISubsystem.h"
#include "Widgets/ConfirmationScreen/PxiiConfirmationPopupScreen.h"
#include "PxiiPushConfirmationScreenAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonClicked, EConfirmationScreenButtonAction, ButtonAction);

/**
 * 
 */
UCLASS()
class PXII_API UPxiiPushConfirmationScreenAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	//~ Begin UBlueprintAsyncActionBase Interface
	virtual void Activate() override;
	//~ End UBlueprintAsyncActionBase Interface
	
	UFUNCTION(BlueprintCallable, Category = "UI | Async Action", meta = (WorldContext = "WorldContextObject",
		HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Show Confirmation Screen"))
	static UPxiiPushConfirmationScreenAsyncAction* PushConfirmScreen(const UObject* WorldContextObject,
																	const FText InScreenTitle, const FText InScreenContent, 
																	TArray<FConfirmationScreenButtonInfo> ButtonsToCreate);	
	
	UPROPERTY(BlueprintAssignable)
	FOnButtonClicked OnButtonClicked;
	
private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	FText CachedScreenTitle;
	FText CachedScreenContent;
	TArray<FConfirmationScreenButtonInfo> CachedButtonsToCreate;
};
