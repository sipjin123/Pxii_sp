#include "Utility/PXIILogUtility.h"

DEFINE_LOG_CATEGORY(PXIILog);
DEFINE_LOG_CATEGORY(PXIISocketLogs);
DEFINE_LOG_CATEGORY(PXIILobbyLogs);
DEFINE_LOG_CATEGORY(PXIIInventoryLogs);
DEFINE_LOG_CATEGORY(PXIIAILogs);
DEFINE_LOG_CATEGORY(PXIICombatLogs);
DEFINE_LOG_CATEGORY(PXIIAbilityLogs);
DEFINE_LOG_CATEGORY(PXIIEquipmentLogs);
DEFINE_LOG_CATEGORY(PXIISkilltreeLogs);
DEFINE_LOG_CATEGORY(PXIIObjectiveLogs);
DEFINE_LOG_CATEGORY(PXIIProjectileLogs);
DEFINE_LOG_CATEGORY(PXIITraceLogs);
DEFINE_LOG_CATEGORY(PXIIAimLogs);
DEFINE_LOG_CATEGORY(PXIIUILogs);
DEFINE_LOG_CATEGORY(PXIIControlsLogs);
DEFINE_LOG_CATEGORY(PXIILedgeTraversalLogs);


void PXIILogUtility::Enable(ELogCategory Category)
{
	EnabledCategories.Add(Category);
}

void PXIILogUtility::Disable(ELogCategory Category)
{
	EnabledCategories.Remove(Category);
}

bool PXIILogUtility::IsEnabled(ELogCategory Category)
{
	return EnabledCategories.Contains(Category);
}

TSet<ELogCategory> PXIILogUtility::EnabledCategories =
{
	ELogCategory::General,
	ELogCategory::Socket,
	ELogCategory::Lobby,
	//ELogCategory::Inventory,
	//ELogCategory::AI,
	//ELogCategory::Combat,
	ELogCategory::Ability,
	ELogCategory::Equipment,
	ELogCategory::SkillTree,
	ELogCategory::LedgeTraversal,
	//ELogCategory::Objective,
};
