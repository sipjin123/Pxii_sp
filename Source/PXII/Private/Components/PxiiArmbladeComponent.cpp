#include "Components/PxiiArmbladeComponent.h"
#include "Data/PxiiArmbladeEffectData.h"
#include "Kismet/GameplayStatics.h"

void UPxiiArmbladeComponent::PlayAttackGrunt_Implementation()
{
	if(PlayerSfxData == nullptr)
	{
		return;
	}
	
	FAttackGruntSfx attackGrunt = PlayerSfxData->PlayerAttackGrunt;
	if(attackGrunt.GruntSfx.IsEmpty())
	{
		return;
	}

	
	int32 soundIndex = FMath::RandRange(0, attackGrunt.GruntSfx.Num() - 1);
	USoundBase* soundCue = attackGrunt.GruntSfx[soundIndex];
	if(!soundCue)
	{
		return;
	}

	const float MinVolume = FMath::Min(attackGrunt.MinVolumeMultiplier, attackGrunt.MaxVolumeMultiplier);
	const float MaxVolume = FMath::Max(attackGrunt.MinVolumeMultiplier, attackGrunt.MaxVolumeMultiplier);

	const float VolumeMult = FMath::FRandRange(MinVolume, MaxVolume);
	
	const float MinPitch = FMath::Min(attackGrunt.MinPitchMultiplier, attackGrunt.MaxPitchMultiplier);
	const float MaxPitch = FMath::Max(attackGrunt.MinPitchMultiplier, attackGrunt.MaxPitchMultiplier);

	const float PitchMult = FMath::FRandRange(MinPitch, MaxPitch);

	UGameplayStatics::PlaySoundAtLocation(
		this,
		soundCue,
		GetOwner()->GetActorLocation(),
		FRotator::ZeroRotator,
		VolumeMult,       // Volume multiplier
		PitchMult       // Pitch multiplier
	);
}

void UPxiiArmbladeComponent::ApplyHitStop_Implementation(EHitImpact hitImpact)
{
	HitImpactType = hitImpact;
}

void UPxiiArmbladeComponent::ResetHitStop_Implementation()
{
	
}

void UPxiiArmbladeComponent::ShowImpactVfx_Implementation(const FVector& impactLocation, const FVector& hitDirection)
{
}
