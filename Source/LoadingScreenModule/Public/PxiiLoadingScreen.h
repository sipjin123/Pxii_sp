// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/CurveSequence.h"
#include "Brushes/SlateDynamicImageBrush.h"
#include "PxiiLoadingScreenTableRow.h"
#include "Engine/StreamableManager.h"
#include "Slate/DeferredCleanupSlateBrush.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SBorder.h"

/**
 * 
 */
class LOADINGSCREENMODULE_API SPxiiLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPxiiLoadingScreen){}
		SLATE_ARGUMENT(const FPxiiLoadingScreenTableRow*, DataRow)
		SLATE_ARGUMENT(FName, LoadedMapName)
		SLATE_ARGUMENT(TSharedPtr<FStreamableHandle>, MapLoadHandle)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	//~ Begin SWidget Interface
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	//~ End SWidget Interface
	
	void SetLoadHandle(TSharedPtr<FStreamableHandle> InHandle) { CurrentLoadHandle = InHandle; }
	void StartFadeIn();
	void StartFadeOut();
	
private:
	void CreateMessageQueue();
	void RandomBackgroundBrush();
	
	const FSlateBrush* GetBackgroundBrush() const;
	FText GetCurrentMessage() const;
	FLinearColor GetMessageTint() const;
	TOptional<float> GetProgressAttribute() const;
	FText GetProgressText() const;
	
	const FPxiiLoadingScreenTableRow* CurrentDataRow = nullptr;
	FName CurrentLoadedMapName;
	TSharedPtr<FStreamableHandle> CurrentLoadHandle;
	
	TSharedPtr<FDeferredCleanupSlateBrush> BackgroundBrush;
	
	TArray<FText> MessageQueue;
	int32 CurrentMessageIndex = 0;
	float CountDownToNextMessage = 0.f;
	
	TSharedPtr<SBorder> BackgroundBorder;
	
	float DisplayProgress = 0.f;
	bool bHasProgress = false;
	bool bIsFading = false;
	bool bFadeOut = false;
	float CurrentScreenOpacity = 1.f;
	float ScreenShowTime = 0.f;
	
	FTextBlockStyle CachedMessageTextStyle;
	FTextBlockStyle CachedProgressTextStyle;
	FProgressBarStyle CachedBarStyle;
};
