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

UAnimMontage* UPxiiEnemyAnimationHandler::GetBlockFlinchAnimation()
{
	if(AnimationData == nullptr)
	{
		return nullptr;
	}

	auto data = AnimationData->BlockFlinch;
    if(data.IsEmpty())
    {
    	return nullptr;
    }

	if(data.Num() == 1)
	{
		return data[0];
	}
	
	int randIndex = FMath::RandRange(0, data.Num() - 1);
	return data[randIndex];
}

UAnimMontage* UPxiiEnemyAnimationHandler::GetDeathAnimation()
{
	if(AnimationData == nullptr)
	{
		return nullptr;
		
	}
	UAnimMontage* data = AnimationData->DeathMontage;
	return data;
}
