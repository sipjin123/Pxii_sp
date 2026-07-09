// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Widgets/PxiiWidgetStackBase.h"
#include "GameplayTagContainer.h"
#include "CommonUI/PxiiActivatableWidget.h"
#include "PxiiUISubsystem.generated.h"

UENUM(BlueprintType)
enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
};

/**
 * 
 */
UCLASS()
class PXII_API UPxiiUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UPxiiUISubsystem* Get(const UObject* WorldContextObject);

	//~ Begin USubsystem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~ End USubsystem Interface

	UFUNCTION(BlueprintCallable)
	void RegisterBaseStack(UPxiiWidgetStackBase* InBaseStack);

	//Need to specify the Soft Widget class in project settings
	void PushSoftWidgetToStackAsync(const FGameplayTag& InStackTag, TSoftClassPtr<UPxiiActivatableWidget> InSoftWidgetClass, TFunction<void(EAsyncPushWidgetState, UPxiiActivatableWidget*)> InAsyncPushStateCallback);

private:
	UPROPERTY(Transient)
	UPxiiWidgetStackBase* CachedStack;
};
