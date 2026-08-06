#include "Combat/PxiiDpsProjectile.h"
#include "Components/PxiiCombatComponent.h"
#include "Components/PxiiPlayerCombatComponent.h"

void APxiiDpsProjectile::ApplyDamageEffectToActor_Implementation(AActor* TargetActor, const FHitResult& result)
{
	Super::ApplyDamageEffectToActor_Implementation(TargetActor, result);

	UPxiiCombatComponent* SelfCombatComp = IPxiiCombatInterface::Execute_GetCombatComponent(WeaponOwner);
	if (!SelfCombatComp)
	{
		return;   
	}
	if (UPxiiPlayerCombatComponent* PlayerCombatComp = Cast<UPxiiPlayerCombatComponent>(SelfCombatComp))
	{
		PlayerCombatComp->ProcessDPSDamage(TargetActor, result.ImpactPoint, DPSData,  EDamageSource::Range);
	}
}

void APxiiDpsProjectile::SetDpsData(const FDpsData& data)
{
	DPSData = data;
	isDpsSet = true;
}

void APxiiDpsProjectile::ReturnProjecileToPool_Implementation()
{
	isDpsSet = false;
	Super::ReturnProjecileToPool_Implementation();
}
