#pragma once
#include "Utility/PXIILogUtility.h"
#include "Runtime/Engine/Classes/Engine/DataAsset.h"
#include "PxiiLogData.generated.h"

UCLASS(BlueprintType)
class PXII_API UPxiiLogData : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly)
	TMap<ELogCategory, bool> LogCategories =
	{
		{ ELogCategory::General,    true },
		{ ELogCategory::Socket,     true },
		{ ELogCategory::Lobby,      true },
		{ ELogCategory::Inventory,  true },
		{ ELogCategory::AI,         true },
		{ ELogCategory::Combat,     true },
		{ ELogCategory::Ability,    true },
		{ ELogCategory::Equipment,  true },
		{ ELogCategory::SkillTree,  true },
		{ ELogCategory::Objective,  true },
		{ ELogCategory::UI,         true },
	};
};

