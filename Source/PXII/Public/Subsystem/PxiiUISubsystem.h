// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Widgets/PxiiWidgetStackBase.h"
#include "GameplayTagContainer.h"
#include "CommonUI/PxiiActivatableWidget.h"
#include "Widgets/Components/PxiiButtonBase.h"
#include "Widgets/Components/PxiiUIClassesMapping.h"
#include "PxiiUISubsystem.generated.h"

UENUM(BlueprintType)
enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdated, UPxiiButtonBase*, BroadcastingButton, FText, DescriptionText);

/**
 * 
 */
UCLASS()
class PXII_API UPxiiUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UPxiiUISubsystem* Get(const UObject* WorldContextObject);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPxiiUIClassesMapping> DataMap;
	
	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdated OnButtonTextDescriptionUpdated;

	//~ Begin USubsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~ End USubsystem Interface

	UFUNCTION(BlueprintCallable, Category = "UISubsystem")
	void RegisterBaseStack(UPxiiWidgetStackBase* InBaseStack);

	//Need to specify the Soft Widget class in project settings
	void PushSoftWidgetToStackAsync(const FGameplayTag& InStackTag, TSoftClassPtr<UPxiiActivatableWidget> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UPxiiActivatableWidget*)> InAsyncPushStateCallback);
	
	UFUNCTION(BlueprintPure, Category = "UISubsystem")
	TSoftClassPtr<UPxiiActivatableWidget> GetWidgetSoftClassByTag(UPARAM(meta = (Categories = "Pxii.UI.Widget")) const FGameplayTag InWidgetTag);
	
private:
	UPROPERTY(Transient)
	UPxiiWidgetStackBase* CachedStack;
};
