// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonUI/PxiiCommonUserWidget.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "PxiiWidgetStackBase.generated.h"

/**
 *
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class PXII_API UPxiiWidgetStackBase : public UPxiiCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI | Widget Stack")
	UCommonActivatableWidgetContainerBase* FindStackByTag(const FGameplayTag& InStackTag) const;

protected:
	UFUNCTION(BlueprintCallable, Category = "UI | Widget Stack")
	void RegisterWidgetStack(UPARAM(meta = (Categories = "Pxii.UI.WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InWidgetStack);

private:
	UPROPERTY(Transient)
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisteredWidgetStackMap;
};
