// Fill out your copyright notice in the Description page of Project Settings.

#include "PxiiLoadingScreenStyle.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Widgets/Notifications/SProgressBar.h"

TSharedPtr<FSlateStyleSet> FPxiiLoadingScreenStyle::StyleInstance = nullptr;

void FPxiiLoadingScreenStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FPxiiLoadingScreenStyle::Shutdown()
{
	if (StyleInstance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
		ensure(StyleInstance.IsUnique());
		StyleInstance.Reset();
	}
}

void FPxiiLoadingScreenStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FPxiiLoadingScreenStyle::Get()
{
	return *StyleInstance;
}

FName FPxiiLoadingScreenStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("PxiiLoadingScreenStyle"));
	return StyleSetName;
}

TSharedRef<class FSlateStyleSet> FPxiiLoadingScreenStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	
	Style->SetContentRoot(FPaths::ProjectContentDir() / TEXT("Developers/Danny/LoadingScreen"));
	Style->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));
	
	// Background - when there is no background assigned in table row
	Style->Set("LoadingScreen.DefaultBackground",
		new FSlateImageBrush(Style->RootToContentDir(TEXT("DefaultBackground"), TEXT(".png")),
			FVector2D(1920.f, 1080.f)));
	
	// Loading bar
	Style->Set("LoadingScreen.ProgressBar",
		 FProgressBarStyle()
			.SetBackgroundImage(FSlateBoxBrush(Style->RootToContentDir(TEXT("ProgressBarBackground"), TEXT(".png")), FMargin(4.f / 22.f)))
			.SetFillImage(FSlateBoxBrush(Style->RootToContentDir(TEXT("BarFill"), TEXT(".png")), FMargin(4.f / 22.f))));
	
	// Message
	Style->Set("LoadingScreen.MessageText",
	FTextBlockStyle()
		.SetFont(FSlateFontInfo(Style->RootToContentDir(TEXT("Fonts/Roboto-Regular"), TEXT(".ttf")), 18))
		.SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 0.9f)))
		.SetShadowOffset(FVector2D(1.f, 1.f))
		.SetShadowColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 0.6f)));
	
	return Style;
}
