#pragma once
#include "Engine/Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(PXIILog, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PXIISocketLogs, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PXIILobbyLogs, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PXIIInventoryLogs, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PXIIAILogs, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PXIICombatLogs, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PXIIAbilityLogs, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PXIIEquipmentLogs, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PXIISkilltreeLogs, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PXIIObjectiveLogs, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PXIIControlsLogs, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PXIIUILogs, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PXIIAimLogs, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PXIITraceLogs, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PXIIProjectileLogs, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(PXIILedgeTraversalLogs, Log, All);

UENUM()
enum class EPXIILogVerbosity : uint8
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
enum class ELogCategory : uint8
{
	General,
	Socket,
	Lobby,
	Inventory,
	AI,
	Combat,
	Ability,
	Equipment,
	SkillTree,
	Objective,
	Controls,
	UI,
	Aim,
	Trace,
	Projectile,
	LedgeTraversal
};

class PXII_API PXIILogUtility
{
public:
	static TSet<ELogCategory> EnabledCategories;

	static void Enable(ELogCategory Category);

	static void Disable(ELogCategory Category);

	static bool IsEnabled(ELogCategory Category);
	
	static FLogCategoryBase& GetLogCategory(ELogCategory category)
	{
		switch (category)
		{
		case ELogCategory::General:			return PXIILog;
		case ELogCategory::Socket:			return PXIISocketLogs;
		case ELogCategory::Lobby:			return PXIILobbyLogs;
		case ELogCategory::Inventory:		return PXIIInventoryLogs;
		case ELogCategory::AI:				return PXIIAILogs;
		case ELogCategory::Combat:			return PXIICombatLogs;
		case ELogCategory::Ability:			return PXIIAbilityLogs;
		case ELogCategory::Equipment:		return PXIIEquipmentLogs;
		case ELogCategory::SkillTree:		return PXIISkilltreeLogs;
		case ELogCategory::Objective:		return PXIIObjectiveLogs;
	
		case ELogCategory::Controls:		return PXIIControlsLogs;
		case ELogCategory::UI:				return PXIIUILogs;
		case ELogCategory::Trace:			return PXIITraceLogs;
		case ELogCategory::Projectile:		return PXIIProjectileLogs;
		case ELogCategory::Aim:				return PXIIAimLogs;
		case ELogCategory::LedgeTraversal:	return PXIILedgeTraversalLogs;
		default:							return PXIILog;
		}
	}
};

#define PXII_LOG(CategoryEnum, Verbosity, Format, ...) \
do { \
	if (PXIILogUtility::IsEnabled(CategoryEnum)) \
	{ \
		switch (CategoryEnum) \
		{ \
			case ELogCategory::General: \
				UE_LOG(PXIILog, Verbosity, Format, ##__VA_ARGS__); \
				break; \
			case ELogCategory::Socket: \
				UE_LOG(PXIISocketLogs, Verbosity, Format, ##__VA_ARGS__); \
				break; \
			case ELogCategory::Lobby: \
				UE_LOG(PXIILobbyLogs, Verbosity, Format, ##__VA_ARGS__); \
				break; \
			case ELogCategory::Inventory: \
				UE_LOG(PXIIInventoryLogs, Verbosity, Format, ##__VA_ARGS__); \
				break; \
			case ELogCategory::AI: \
				UE_LOG(PXIIAILogs, Verbosity, Format, ##__VA_ARGS__); \
				break; \
			case ELogCategory::Combat: \
				UE_LOG(PXIICombatLogs, Verbosity, Format, ##__VA_ARGS__); \
				break; \
			case ELogCategory::Ability: \
				UE_LOG(PXIIAbilityLogs, Verbosity, Format, ##__VA_ARGS__); \
				break; \
			case ELogCategory::Equipment: \
				UE_LOG(PXIIEquipmentLogs, Verbosity, Format, ##__VA_ARGS__); \
				break; \
			case ELogCategory::SkillTree: \
				UE_LOG(PXIISkilltreeLogs, Verbosity, Format, ##__VA_ARGS__); \
				break; \
			case ELogCategory::Objective: \
				UE_LOG(PXIIObjectiveLogs, Verbosity, Format, ##__VA_ARGS__); \
				break; \
			case ELogCategory::Controls: \
				UE_LOG(PXIIControlsLogs, Verbosity, Format, ##__VA_ARGS__); \
			break; \
			case ELogCategory::UI: \
				UE_LOG(PXIIUILogs, Verbosity, Format, ##__VA_ARGS__); \
			break; \
			case ELogCategory::Trace: \
				UE_LOG(PXIITraceLogs, Verbosity, Format, ##__VA_ARGS__); \
			break; \
			case ELogCategory::Aim: \
				UE_LOG(PXIIAimLogs, Verbosity, Format, ##__VA_ARGS__); \
			break; \
			case ELogCategory::Projectile: \
				UE_LOG(PXIIProjectileLogs, Verbosity, Format, ##__VA_ARGS__); \
			break; \
			case ELogCategory::LedgeTraversal: \
			UE_LOG(PXIILedgeTraversalLogs, Verbosity, Format, ##__VA_ARGS__); \
			break; \
			default: \
				UE_LOG(LogTemp, Verbosity, Format, ##__VA_ARGS__); \
				break; \
		} \
	} \
} while(0)

static void PrintLogDifferentVerbosity(const FString& Message, ELogCategory LogCategory, EPXIILogVerbosity LogVerbosity)
{
	switch (LogVerbosity)
	{
	case EPXIILogVerbosity::Log			: PXII_LOG(LogCategory, Log, TEXT("%s"), *Message);			break;
	case EPXIILogVerbosity::Display		: PXII_LOG(LogCategory, Display, TEXT("%s"), *Message);		break;
	case EPXIILogVerbosity::Warning		: PXII_LOG(LogCategory, Warning, TEXT("%s"), *Message);		break;
	case EPXIILogVerbosity::Error		: PXII_LOG(LogCategory, Error, TEXT("%s"), *Message);		break;
	case EPXIILogVerbosity::Verbose		: PXII_LOG(LogCategory, Verbose, TEXT("%s"), *Message);		break;
	case EPXIILogVerbosity::VeryVerbose : PXII_LOG(LogCategory, VeryVerbose, TEXT("%s"), *Message);	break;
	case EPXIILogVerbosity::Fatal		: PXII_LOG(LogCategory, Fatal, TEXT("%s"), *Message);		break;
	default								: PXII_LOG(LogCategory, Log, TEXT("%s"), *Message);			break;
	}
};

namespace PxiiLog
{
	static void Print(const FString& ContextName, const FString& Message, ELogCategory LogCategory = ELogCategory::General, EPXIILogVerbosity LogVerbosity = EPXIILogVerbosity::Log, const FColor& Color = FColor::MakeRandomColor(), float DisplayTime = 2.f, int32 InKey = -1)
	{
		if (GEngine)
		{
			const FString FinalMsg = FString::Printf(TEXT("[%s]: %s"), *ContextName, *Message);
			
			GEngine->AddOnScreenDebugMessage(InKey, DisplayTime, Color, FinalMsg);

			PrintLogDifferentVerbosity(FinalMsg, LogCategory, LogVerbosity);
		}
	}

	static void Print(const FString& ContextName, float FloatValueToPrint, ELogCategory LogCategory = ELogCategory::General, EPXIILogVerbosity LogVerbosity = EPXIILogVerbosity::Log, const FColor& Color = FColor::MakeRandomColor(), float DisplayTime = 2.f, int32 InKey = -1)
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