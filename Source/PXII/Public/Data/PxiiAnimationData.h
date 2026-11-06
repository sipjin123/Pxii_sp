#pragma once
#include "Components/PxiiHitFeedbackComponent.h"
#include "PxiiAnimationData.generated.h"

UCLASS(BlueprintType)
class PXII_API UPxiiAnimationData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EHitDirection, TObjectPtr<UAnimMontage>> FlinchMap;
};
