#include "PxiiLoadingScreenLogUtility.h"

DEFINE_LOG_CATEGORY(PXIILoadingScreenLog);
DEFINE_LOG_CATEGORY(PXIISocketLogs);
DEFINE_LOG_CATEGORY(PXIILobbyLogs);
DEFINE_LOG_CATEGORY(PXIIInventoryLogs);
DEFINE_LOG_CATEGORY(PXIIAILogs);
DEFINE_LOG_CATEGORY(PXIICombatLogs);
DEFINE_LOG_CATEGORY(PXIIAbilityLogs);
DEFINE_LOG_CATEGORY(PXIIEquipmentLogs);
DEFINE_LOG_CATEGORY(PXIISkilltreeLogs);
DEFINE_LOG_CATEGORY(PXIIObjectiveLogs);

void PXIILoadingScreenLogUtility::Enable(ELoadingScreenLogCategory Category)
{
	EnabledCategories.Add(Category);
}

void PXIILoadingScreenLogUtility::Disable(ELoadingScreenLogCategory Category)
{
	EnabledCategories.Remove(Category);
}

bool PXIILoadingScreenLogUtility::IsEnabled(ELoadingScreenLogCategory Category)
{
	return EnabledCategories.Contains(Category);
}

TSet<ELoadingScreenLogCategory> PXIILoadingScreenLogUtility::EnabledCategories =
{
	ELoadingScreenLogCategory::General,
	ELoadingScreenLogCategory::Socket,
	ELoadingScreenLogCategory::Lobby,
	//ELogCategory::Inventory,
	//ELogCategory::AI,
	//ELogCategory::Combat,
	ELoadingScreenLogCategory::Ability,
	ELoadingScreenLogCategory::Equipment,
	ELoadingScreenLogCategory::SkillTree,
	//ELogCategory::Objective,
};
