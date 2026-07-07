// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/PxiiWidgetBase.h"
#include "GameplayTagContainer.h"
#include "Components/Widget.h"
#include "PxiiWidgetStackBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class PXII_API UPxiiWidgetStackBase : public UPxiiWidgetBase
{
	GENERATED_BODY()
	
public:
	// Debug
	UFUNCTION(BlueprintCallable, Category = "UI | Widget Stack")
	UUserWidget* PushWidgetToStack(TSubclassOf<UUserWidget> WidgetClass, FGameplayTag InStackTag, bool bActivateOnCreate);

	UFUNCTION(BlueprintCallable, Category = "UI | Widget Stack")
	void PopWidgetFromStack(FGameplayTag InStackTag);

	UFUNCTION(BlueprintCallable, Category = "UI | Widget Stack")
	void PopAllWidget();

	UFUNCTION(BlueprintCallable, Category = "UI | Widget Stack")
	UUserWidget* GetTopMostWidget() const;

	UFUNCTION(BlueprintCallable, Category = "UI | Widget Stack")
	FORCEINLINE int32 GetStackSize() const { return  RegisteredWidgetStack.Num(); }
	// Debug

	UFUNCTION(BlueprintCallable, Category = "UI | Widget Stack")
	UWidget* FindStackByTag(const FGameplayTag& InStackTag) const;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "UI | Widget Stack")
	void RegisterWidgetStack(UPARAM(meta = (Categories = "Pxii.UI.WidgetStack")) FGameplayTag InStackTag, UWidget* InWidgetStack);

	// Debug
	// Called when a widget becomes the top of the stack
	UFUNCTION(BlueprintImplementableEvent, Category = "Widget Stack")
	void OnWidgetActivated(UUserWidget* Widget);

	// Called when a widget is no longer the top
	UFUNCTION(BlueprintImplementableEvent, Category = "Widget Stack")
	void OnWidgetDeactivated(UUserWidget* Widget);
	//Debug

private:
	UPROPERTY(Transient)
	TMap<FGameplayTag, UWidget*> RegisteredWidgetStack;
};
