#pragma once
#include "Engine/Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(PXIILoadingScreenLog, Log, All);

UENUM()
enum class EPXIILoadingScreenLogVerbosity : uint8
{
	Log,
	Display,
	Warning,
	Error,
	Verbose,
	VeryVerbose,
	Fatal,
};

UENUM()
enum class ELoadingScreenLogCategory : uint8
{
	General,
};

class LOADINGSCREENMODULE_API PXIILoadingScreenLogUtility
{
public:
	static TSet<ELoadingScreenLogCategory> EnabledCategories;

	static void Enable(ELoadingScreenLogCategory Category);

	static void Disable(ELoadingScreenLogCategory Category);

	static bool IsEnabled(ELoadingScreenLogCategory Category);
	
	static FLogCategoryBase& GetLogCategory(ELoadingScreenLogCategory category)
	{
		switch (category)
		{
			case ELoadingScreenLogCategory::General:    return PXIILoadingScreenLog;
			default:									return PXIILoadingScreenLog;
		}
	}
};

#define PXII_LOADINGSCREEN_LOG(CategoryEnum, Verbosity, Format, ...) \
do { \
	if (PXIILoadingScreenLogUtility::IsEnabled(CategoryEnum)) \
	{ \
		switch (CategoryEnum) \
		{ \
			case ELoadingScreenLogCategory::General: \
				UE_LOG(PXIILoadingScreenLog, Verbosity, Format, ##__VA_ARGS__); \
				break; \
			default: \
				UE_LOG(LogTemp, Verbosity, Format, ##__VA_ARGS__); \
				break; \
		} \
	} \
} while(0)

static void PrintLogDifferentVerbosity(const FString& Message, ELoadingScreenLogCategory LogCategory, EPXIILoadingScreenLogVerbosity LogVerbosity)
{
	switch (LogVerbosity)
	{
	case EPXIILoadingScreenLogVerbosity::Log			: PXII_LOADINGSCREEN_LOG(LogCategory, Log, TEXT("%s"), *Message);			break;
	case EPXIILoadingScreenLogVerbosity::Display		: PXII_LOADINGSCREEN_LOG(LogCategory, Display, TEXT("%s"), *Message);		break;
	case EPXIILoadingScreenLogVerbosity::Warning		: PXII_LOADINGSCREEN_LOG(LogCategory, Warning, TEXT("%s"), *Message);		break;
	case EPXIILoadingScreenLogVerbosity::Error		:PXII_LOADINGSCREEN_LOG(LogCategory, Error, TEXT("%s"), *Message);		break;
	case EPXIILoadingScreenLogVerbosity::Verbose		: PXII_LOADINGSCREEN_LOG(LogCategory, Verbose, TEXT("%s"), *Message);		break;
	case EPXIILoadingScreenLogVerbosity::VeryVerbose : PXII_LOADINGSCREEN_LOG(LogCategory, VeryVerbose, TEXT("%s"), *Message);	break;
	case EPXIILoadingScreenLogVerbosity::Fatal		: PXII_LOADINGSCREEN_LOG(LogCategory, Fatal, TEXT("%s"), *Message);		break;
	default								: PXII_LOADINGSCREEN_LOG(LogCategory, Log, TEXT("%s"), *Message);			break;
	}
};

namespace PxiiLoadingScreenLog
{
	static void Print(const FString& ContextName, const FString& Message, ELoadingScreenLogCategory LogCategory = ELoadingScreenLogCategory::General, EPXIILoadingScreenLogVerbosity LogVerbosity = EPXIILoadingScreenLogVerbosity::Log, const FColor& Color = FColor::MakeRandomColor(), float DisplayTime = 2.f, int32 InKey = -1)
	{
		if (GEngine)
		{
			const FString FinalMsg = FString::Printf(TEXT("[%s]: %s"), *ContextName, *Message);
			
			GEngine->AddOnScreenDebugMessage(InKey, DisplayTime, Color, FinalMsg);

			PrintLogDifferentVerbosity(FinalMsg, LogCategory, LogVerbosity);
		}
	}

	static void Print(const FString& ContextName, float FloatValueToPrint, ELoadingScreenLogCategory LogCategory = ELoadingScreenLogCategory::General, EPXIILoadingScreenLogVerbosity LogVerbosity = EPXIILoadingScreenLogVerbosity::Log, const FColor& Color = FColor::MakeRandomColor(), float DisplayTime = 2.f, int32 InKey = -1)
	{
		if (GEngine)
		{
			const FString FinalMsg = FString::Printf(TEXT("[%s]: %s"), *ContextName, *FString::SanitizeFloat(FloatValueToPrint));
			
			GEngine->AddOnScreenDebugMessage(InKey, DisplayTime, Color, FinalMsg);

			PrintLogDifferentVerbosity(FinalMsg, LogCategory, LogVerbosity);
		}
	}

	// Only on screen msg, no log
	static void PrintOnScreen(const FString& ContextName, const FString& Message, const FColor& Color = FColor::MakeRandomColor(), float DisplayTime = 2.f, int32 InKey = -1)
	{
		if (GEngine)
		{
			const FString FinalMsg = FString::Printf(TEXT("[%s]: %s"), *ContextName, *Message);
			
			GEngine->AddOnScreenDebugMessage(InKey, DisplayTime, Color, FinalMsg);
		}
	}
};