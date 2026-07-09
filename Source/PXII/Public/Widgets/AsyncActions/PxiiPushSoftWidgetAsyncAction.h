// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CommonUI/PxiiActivatableWidget.h"
#include "PxiiPushSoftWidgetAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidget, UPxiiActivatableWidget*, PushedWidget);
/**
 * 
 */
UCLASS()
class PXII_API UPxiiPushSoftWidgetAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	//~ Begin UBlueprintAsyncActionBase Interface
	virtual void Activate() override;
	//~ End UBlueprintAsyncActionBase Interface

	UFUNCTION(BlueprintCallable, Category = "UI | Async Action", meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Push Soft Widget To Stack"))
	static UPxiiPushSoftWidgetAsyncAction* PushSoftWidget(const UObject* WorldContextObject,
														  APxiiPlayerController* OwningPlayerController, 
														  TSoftClassPtr<UPxiiActivatableWidget> InSoftWidgetClass,
														  UPARAM(meta = (Categories = "Pxii.UI.WidgetStack")) FGameplayTag InStackTag, bool bFocusOnNewlyPushedWidget = true);

#pragma region Delegates
	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidget OnWidgetCreatedBeforePush;

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidget OnAfterPushedWidget;
#pragma endregion

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	TWeakObjectPtr<APxiiPlayerController> CachedOwningPlayerController;
	TSoftClassPtr<UPxiiActivatableWidget> CachedSoftWidgetClass;
	FGameplayTag CachedStackTag;
	bool bCachedFocusOnNewlyPushedWidget = false;
};
