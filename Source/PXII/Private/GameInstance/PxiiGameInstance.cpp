#include "GameInstance/PxiiGameInstance.h"

void UPxiiGameInstance::Init()
{
	Super::Init();

	for (const TPair<ELogCategory, bool>& Pair : LogData->LogCategories)
	{
		ELogCategory Category = Pair.Key;
		if(Pair.Value)
		{
			PXIILogUtility::Enable(Category);
		}
		else
		{
			PXIILogUtility::Disable(Category);
		}
		// Do something
	}
}
