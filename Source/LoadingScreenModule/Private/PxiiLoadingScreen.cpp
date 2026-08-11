// Fill out your copyright notice in the Description page of Project Settings.

#include "PxiiLoadingScreen.h"
#include "SlateOptMacros.h"
#include "Engine/Texture2D.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"
#include "PXIILoadingScreenLogUtility.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "PxiiLoadingScreenStyle.h"
#include "Widgets/Text/STextBlock.h"
#include "CommonTextBlock.h"
#include "MoviePlayer.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

// Helper to extract styles from CommonUI CommonTextStyle
namespace
{
	FTextBlockStyle ConvertFromCommonTextStyle(TSubclassOf<UCommonTextStyle> Style, FName FallbackStyleName)
	{
		if (Style)
		{
			if (const UCommonTextStyle* CommonStyle = GetDefault<UCommonTextStyle>(Style))
			{
				FTextBlockStyle ConvertedStyle;
				CommonStyle->ToTextBlockStyle(ConvertedStyle);
				return ConvertedStyle;
			}
		}
		return FPxiiLoadingScreenStyle::Get().GetWidgetStyle<FTextBlockStyle>(FallbackStyleName);
	}
}

void SPxiiLoadingScreen::Construct(const FArguments& InArgs)
{
	CurrentDataRow = InArgs._DataRow;
	CurrentLoadedMapName = InArgs._LoadedMapName;
	CurrentLoadHandle = InArgs._MapLoadHandle;
	
	ScreenShowTime = FPlatformTime::Seconds();
	DisplayProgress = 0.f;
	
	CreateMessageQueue();
	RandomBackgroundBrush();
	
	CachedMessageTextStyle = ConvertFromCommonTextStyle(CurrentDataRow ? CurrentDataRow->MessageTextStyleClass : nullptr, "LoadingScreen.MessageText");
	CachedProgressTextStyle = ConvertFromCommonTextStyle(CurrentDataRow? CurrentDataRow->ProgressBarTextStyleClass : nullptr, "LoadingScreen.MessageText");
	
	if (CurrentDataRow && !CurrentDataRow->ProgressBarStyleAsset.IsNull())
	{
		if (UPxiiLoadingBarStyle* LoadingBarStyle = CurrentDataRow->ProgressBarStyleAsset.LoadSynchronous())
		{
			CachedBarStyle = LoadingBarStyle->ProgressBarStyle;
		}
		else
		{
			CachedBarStyle = FPxiiLoadingScreenStyle::Get().GetWidgetStyle<FProgressBarStyle>("LoadingScreen.ProgressBar");
		}
	}
	
	const FProgressBarStyle* ProgressBarStyle = &CachedBarStyle;
	StartFadeIn();
	
	ChildSlot
	[
		// Populate the widget
		// Background
		SAssignNew(BackgroundBorder, SBorder)
		.BorderImage(FStyleDefaults::GetNoBrush())
		.ColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, CurrentScreenOpacity))
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
			[
				SNew(SImage).Image(this, &SPxiiLoadingScreen::GetBackgroundBrush)
			]

			// The bottom section
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)
			.Padding(200.f, 0.f, 200.f, 100.f)
			[
				SNew(SHorizontalBox)
				
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Bottom)
				[
					SNew(STextBlock)
					.AutoWrapText(true)
					.Text(this, &SPxiiLoadingScreen::GetCurrentMessage)
					.TextStyle(&CachedMessageTextStyle)
					.ColorAndOpacity(FSlateColor(GetMessageTint()))
				]

				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Bottom)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Right)
					.Padding(0.f, 0.f, 0.f, 5.f)
					[
						SNew(STextBlock)
						.Text(this, &SPxiiLoadingScreen::GetProgressText)
						.TextStyle(&CachedProgressTextStyle)
					]
					
					+ SVerticalBox::Slot()
					[
						SNew(SBox)
						.WidthOverride(360.f)
						.HeightOverride(8.f)
						[
							SNew(SProgressBar)
							.Style(ProgressBarStyle)
							.Percent(this, &SPxiiLoadingScreen::GetProgressAttribute)
						]
					]
				]
			]
		]
	];
}

void SPxiiLoadingScreen::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	
	if (MessageQueue.Num() > 1)
	{
		CountDownToNextMessage -= InDeltaTime;
		if (CountDownToNextMessage <= 0.f)
		{
			CurrentMessageIndex = (CurrentMessageIndex + 1) % MessageQueue.Num();
			CountDownToNextMessage = CurrentDataRow ? CurrentDataRow->MessageCycleInterval : 4.0f;
		}
	}
	
	// Get actual loading
	if (CurrentLoadHandle.IsValid() && !bIsFading)
	{
		const float ActualLoadingPercentage = CurrentLoadHandle->GetLoadProgress();

		if (ActualLoadingPercentage >= 0.f)
		{
			PxiiLoadingScreenLog::Print(TEXT("Loading Screen"), FString::Printf(TEXT("%f"), ActualLoadingPercentage));
			DisplayProgress = FMath::Max(DisplayProgress, FMath::FInterpTo(DisplayProgress, ActualLoadingPercentage, InDeltaTime, 5.f));
			bHasProgress = true;
		}
		
		if (DisplayProgress >= 1.f)
		{
			CurrentLoadHandle.Reset();
		}
	}
	
	if (!bFadeOut && !CurrentLoadHandle.IsValid())
	{
		if ((FPlatformTime::Seconds() - ScreenShowTime) >= CurrentDataRow->MinimumDisplayTime)
		{
			StartFadeOut();
		}
	}
	
	if (bIsFading)
	{
		if (!bFadeOut)
		{
			CurrentScreenOpacity += (1.f * InDeltaTime) / .5f;
		}
		else if (bFadeOut)
		{
			CurrentScreenOpacity += (-1.f * InDeltaTime) / .5f;
			PxiiLoadingScreenLog::Print("LoadingScreen: ", FString::Printf(TEXT("Fade out, %f"), CurrentScreenOpacity));
		}
		
		CurrentScreenOpacity = FMath::Clamp(CurrentScreenOpacity, 0.f, 1.f);
		
		if (BackgroundBorder.IsValid())
		{
				BackgroundBorder->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, CurrentScreenOpacity));
		}
		
		if (CurrentScreenOpacity <= 0.f || CurrentScreenOpacity >= 1.f)
		{
			bIsFading = false;
			
			if (bFadeOut && CurrentScreenOpacity <= 0.f)
			{
				GetMoviePlayer()->StopMovie();
				DisplayProgress = 0.f;
				bHasProgress = false;
			}
		}
	}
}

void SPxiiLoadingScreen::CreateMessageQueue()
{
	MessageQueue.Reset();
	
	if (CurrentDataRow && !CurrentDataRow->MessageStringTable.IsNull())
	{
		if (UStringTable* StringTable = CurrentDataRow->MessageStringTable.LoadSynchronous())
		{
			if (TSharedPtr<FStringTable> ActualTable = StringTable->GetMutableStringTable())
			{
				const FName TableID = StringTable->GetStringTableId();
				ActualTable->EnumerateSourceStrings(
					[this, TableID](const FString& Key, const FString&)
					{
						MessageQueue.Add(FText::FromStringTable(TableID, Key));
						return true;
					}	
				);
			}
		}
	}
	
	if (MessageQueue.Num() == 0)
	{
		MessageQueue.Add(NSLOCTEXT("Loading", "Default", "No message assigned...."));
	}
	
	if (CurrentDataRow && CurrentDataRow->bShuffleMessage)
	{
		for (int32 i = MessageQueue.Num() - 1; i >= 0; --i)
		{
			MessageQueue.Swap(i, FMath::RandRange(0, i));
		}
	}
	
	CountDownToNextMessage = CurrentDataRow ? CurrentDataRow->MessageCycleInterval : 4.0f;
}

void SPxiiLoadingScreen::RandomBackgroundBrush()
{
	if (!CurrentDataRow || CurrentDataRow->BackgroundTextures.Num() == 0)
	{
		return;
	}
	
	int32 Index = 0;
	
	if (CurrentDataRow->bRandomizeBackground && CurrentDataRow->BackgroundTextures.Num() > 1)
	{
		Index = FMath::RandRange(0, CurrentDataRow->BackgroundTextures.Num() - 1);
	}
	
	if (UTexture2D* BGTexture = CurrentDataRow->BackgroundTextures[Index].LoadSynchronous())
	{
		BackgroundBrush = FDeferredCleanupSlateBrush::CreateBrush(BGTexture);
	}
}

void SPxiiLoadingScreen::StartFadeIn()
{
	bIsFading = true;
	bFadeOut = false;
	CurrentScreenOpacity = 0.f;
}

void SPxiiLoadingScreen::StartFadeOut()
{ 
	bIsFading = true;
	bFadeOut = true;
	CurrentScreenOpacity = 1.f;
}

const FSlateBrush* SPxiiLoadingScreen::GetBackgroundBrush() const
{
	if (BackgroundBrush.IsValid())
	{
		return BackgroundBrush->GetSlateBrush();
	}
	
	return FPxiiLoadingScreenStyle::Get().GetBrush("LoadingScreen.DefaultBackground");
}

FText SPxiiLoadingScreen::GetCurrentMessage() const
{
	return MessageQueue.IsValidIndex(CurrentMessageIndex) ? MessageQueue[CurrentMessageIndex] : FText::GetEmpty();
}

FLinearColor SPxiiLoadingScreen::GetMessageTint() const
{
	return CachedMessageTextStyle.ColorAndOpacity.GetSpecifiedColor();
}

TOptional<float> SPxiiLoadingScreen::GetProgressAttribute() const
{
	return bHasProgress ? TOptional<float>(DisplayProgress) : TOptional<float>(0);
}

FText SPxiiLoadingScreen::GetProgressText() const
{
	return bHasProgress ? FText::AsPercent(DisplayProgress) : FText::GetEmpty();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
