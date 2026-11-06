#include "Components/PxiiEnemyAnimationHandler.h"

UAnimMontage* UPxiiEnemyAnimationHandler::GetFlinchAnimation(EHitDirection HitDirection)
{
	if(AnimationData == nullptr)
	{
		return nullptr;
	}

	auto data = AnimationData->FlinchMap;
	if(!data.Contains(HitDirection))
	{
		return nullptr;
	}

	return data[HitDirection];
}
