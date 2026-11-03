// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PxiiWeaponMelee.h"

#include "Kismet/GameplayStatics.h"

void APxiiWeaponMelee::PlaySlashSFX()
{
	FWeaponSlashSfx weaponSlashSfx = ArmbladeEffectData->ArmbladeSlashSfx;
	if(ArmbladeEffectData == nullptr)
	{
		return;
	}

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
