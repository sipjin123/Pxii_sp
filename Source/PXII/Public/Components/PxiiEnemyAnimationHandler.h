#pragma once
#include "Data/PxiiAnimationData.h"
#include "PxiiEnemyAnimationHandler.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiEnemyAnimationHandler : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPxiiAnimationData> AnimationData;

	UFUNCTION(BlueprintPure)
	UAnimMontage* GetFlinchAnimation(EHitDirection HitDirection);
};
