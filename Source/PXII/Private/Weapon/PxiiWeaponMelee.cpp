// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PxiiWeaponMelee.h"

#include "Components/PxiiYinYangComponent.h"
#include "Kismet/GameplayStatics.h"

void APxiiWeaponMelee::PlaySlashSFX_Implementation()
{
	if(ArmbladeEffectData == nullptr)
	{
		return;
	}
	
	FWeaponSlashSfx weaponSlashSfx = ArmbladeEffectData->ArmbladeSlashSfx;
	if(weaponSlashSfx.WeaponSfx.IsEmpty())
	{
		return;
	}

	
	int32 soundIndex = FMath::RandRange(0, weaponSlashSfx.WeaponSfx.Num() - 1);
	USoundBase* soundCue = weaponSlashSfx.WeaponSfx[soundIndex];
	if(!soundCue)
	{
		return;
	}

	const float MinVolume = FMath::Min(weaponSlashSfx.MinVolumeMultiplier, weaponSlashSfx.MaxVolumeMultiplier);
	const float MaxVolume = FMath::Max(weaponSlashSfx.MinVolumeMultiplier, weaponSlashSfx.MaxVolumeMultiplier);

	const float VolumeMult = FMath::FRandRange(MinVolume, MaxVolume);
	
	const float MinPitch = FMath::Min(weaponSlashSfx.MinPitchMultiplier, weaponSlashSfx.MaxPitchMultiplier);
	const float MaxPitch = FMath::Max(weaponSlashSfx.MinPitchMultiplier, weaponSlashSfx.MaxPitchMultiplier);

	const float PitchMult = FMath::FRandRange(MinPitch, MaxPitch);

	UGameplayStatics::PlaySoundAtLocation(
		this,
		soundCue,
		GetActorLocation(),
		FRotator::ZeroRotator,
		VolumeMult,       // Volume multiplier
		PitchMult       // Pitch multiplier
	);
	
}

void APxiiWeaponMelee::ApplyCost_Implementation()
{
	if(!Cost)
	{
		return;
	}

	if(!GetOwner())
	{
		return;
	}

	UPxiiYinYangComponent* yinyangComp = GetOwner()->GetComponentByClass<UPxiiYinYangComponent>();
	if(yinyangComp)
	{
		yinyangComp->ApplyCost(Cost->CostType, Cost->CostValue);
	}
}

void APxiiWeaponMelee::PlaySlashImpact_Implementation(EPhysicalSurface SurfaceType, const FVector& impactLocation)
{
	if(ArmbladeEffectData == nullptr)
	{
		return;
	}

	FImpactSoundEffect weaponImpactSfx = GetImpactSoundEffect(SurfaceType);
	if(weaponImpactSfx.ImpactSfx.IsEmpty())
	{
		return;
	}

	
	int32 soundIndex = FMath::RandRange(0, weaponImpactSfx.ImpactSfx.Num() - 1);
	USoundBase* soundCue = weaponImpactSfx.ImpactSfx[soundIndex];
	if(!soundCue)
	{
		return;
	}

	const float MinVolume = FMath::Min(weaponImpactSfx.MinVolumeMultiplier, weaponImpactSfx.MaxVolumeMultiplier);
	const float MaxVolume = FMath::Max(weaponImpactSfx.MinVolumeMultiplier, weaponImpactSfx.MaxVolumeMultiplier);

	const float VolumeMult = FMath::FRandRange(MinVolume, MaxVolume);
	
	const float MinPitch = FMath::Min(weaponImpactSfx.MinPitchMultiplier, weaponImpactSfx.MaxPitchMultiplier);
	const float MaxPitch = FMath::Max(weaponImpactSfx.MinPitchMultiplier, weaponImpactSfx.MaxPitchMultiplier);

	const float PitchMult = FMath::FRandRange(MinPitch, MaxPitch);

	UGameplayStatics::PlaySoundAtLocation(
		this,
		soundCue,
		impactLocation,
		FRotator::ZeroRotator,
		VolumeMult,       // Volume multiplier
		PitchMult       // Pitch multiplier
	);
}

FImpactSoundEffect APxiiWeaponMelee::GetImpactSoundEffect(EPhysicalSurface SurfaceType)
{
	if (const FImpactSoundEffect* Found = ArmbladeEffectData->ArmbladeImpactSFX.Find(SurfaceType))
	{
		return *Found;
	}

	return ArmbladeEffectData->DefaultImpactSFX;
	
}

FHitStopData APxiiWeaponMelee::GetHitStopData(EHitImpact hitImpact)
{
	if(ArmbladeEffectData == nullptr)
	{
		return FHitStopData();
	}

	auto HitStopData = ArmbladeEffectData->HitStopMap;
	if(HitStopData.IsEmpty())
	{
		return FHitStopData();
	}

	if(!HitStopData.Contains(hitImpact))
	{
		auto data = HitStopData.CreateConstIterator();
		auto& defaultValue = data->Value;

		return defaultValue;
	}

	return HitStopData[hitImpact];
}

UCameraShakeAsset* APxiiWeaponMelee::GetCameraShakeImpact(EHitImpact hitImpact)
{
	if(ArmbladeEffectData == nullptr)
	{
		return nullptr;
	}

	auto CameraShake = ArmbladeEffectData->ImpactCameraShake;
	if(CameraShake.IsEmpty())
	{
		return nullptr;
	}

	if(!CameraShake.Contains(hitImpact))
	{
		return nullptr;
	}

	return CameraShake[hitImpact];
}

