#pragma once
#include "Combat/PxiiProjectileBase.h"
#include "Engine/DataAsset.h"
#include "PxiiProjectileMap.generated.h"

UCLASS(BlueprintType)
class PXII_API UPxiiProjectileMap : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectiles | Projectile Classes Soft Reference", meta = (ForceInlineRow, Categories = "Pxii.Projectiles"))
	TMap<FGameplayTag, TSoftClassPtr<APxiiProjectileBase>> ProjectileClassesMap;
};
