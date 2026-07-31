#pragma once

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
	Aim
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
		case ELogCategory::General:     return PXIILog;
		case ELogCategory::Socket:      return PXIISocketLogs;
		case ELogCategory::Lobby:		return PXIILobbyLogs;
		case ELogCategory::Inventory:	return PXIIInventoryLogs;
		case ELogCategory::AI:			return PXIIAILogs;
		case ELogCategory::Combat:		return PXIICombatLogs;
		case ELogCategory::Ability:		return PXIIAbilityLogs;
		case ELogCategory::Equipment:	return PXIIEquipmentLogs;
		case ELogCategory::SkillTree:	return PXIISkilltreeLogs;
		case ELogCategory::Objective:	return PXIIObjectiveLogs;
		default:                         return PXIILog;
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
			default: \
				UE_LOG(LogTemp, Verbosity, Format, ##__VA_ARGS__); \
				break; \
		} \
	} \
} while(0)