#include "PxiiLoadingScreenLogUtility.h"

DEFINE_LOG_CATEGORY(PXIILoadingScreenLog);

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
	//ELogCategory::Objective,
};
