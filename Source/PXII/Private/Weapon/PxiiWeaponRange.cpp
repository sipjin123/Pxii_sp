// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PxiiWeaponRange.h"

#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

APxiiWeaponRange::APxiiWeaponRange()
{

}

void APxiiWeaponRange::BeginPlay()
{
	Super::BeginPlay();

	if(!SKWeapon)
	{
		return;
	}

	if(!SKWeapon->DoesSocketExist(MuzzleSocketName))
	{
		return;
	}

	MuzzleLocation = SKWeapon->GetSocketLocation(MuzzleSocketName);
	
	if(!MuzzleFlashEffect.IsEmpty())
	{
		MuzzleFlashComponents.Empty();
		int32 index = 0;
		for(UNiagaraSystem* system : MuzzleFlashEffect)
		{
			if(!system)
			{
				continue;
			}

			FString vfxName = "MuzzleFlash-" + FString::FromInt(index);
			UNiagaraComponent* MuzzleFlashComponent = NewObject<UNiagaraComponent>(this, FName(*vfxName));
			MuzzleFlashComponent->AttachToComponent(SKWeapon, FAttachmentTransformRules::SnapToTargetIncludingScale, MuzzleSocketName);

			MuzzleFlashComponent->SetupAttachment(SKWeapon, MuzzleSocketName);
			MuzzleFlashComponent->RegisterComponent();

			MuzzleFlashComponent->SetAutoActivate(false);
			MuzzleFlashComponent->SetAsset(system);
			HasMuzzleFlash = true;

			index++;
			MuzzleFlashComponents.Add(MuzzleFlashComponent);
		}
	}
	else
	{
		HasMuzzleFlash = false;
	}
}

void APxiiWeaponRange::PlayMuzzleFlash_Implementation()
{
	if(!HasMuzzleFlash)
	{
		return;
	}

	for(UNiagaraComponent* comp : MuzzleFlashComponents)
	{
		comp->Activate(true);
	}
}

void APxiiWeaponRange::PlayWeaponSfx_Implementation()
{
	if(!WeaponSfx)
	{
		return;
	}
	
	if (!WeaponSfx->FireSfx.IsEmpty())
	{
		int32 sfxIndex = 0;
		if(WeaponSfx->FireSfx.Num() > 1)
		{
			sfxIndex = FMath::RandRange(0, WeaponSfx->FireSfx.Num() - 1);
		}
		FSfxData fireSfx = WeaponSfx->FireSfx[sfxIndex];
		if(fireSfx.Sfx)
		{
			UGameplayStatics::PlaySoundAtLocation(this, fireSfx.Sfx, MuzzleLocation,
				fireSfx.VolumeMultiplier, fireSfx.Pitch, 0.0f);			
		}
	}

	FSfxData click = WeaponSfx->MechanicalClick;
	if (click.Sfx)
	{
		UGameplayStatics::PlaySoundAtLocation(this, click.Sfx, MuzzleLocation,
			click.VolumeMultiplier, click.Pitch, 0.0f);
	}

	FSfxData DistantFireSfx = WeaponSfx->DistantFireSfx;
	if (DistantFireSfx.Sfx)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DistantFireSfx.Sfx, MuzzleLocation,
			DistantFireSfx.VolumeMultiplier, DistantFireSfx.Pitch, 0.0f);
	}
}

UPxiiRecoilPattern* APxiiWeaponRange::GetAdsRecoil()
{
	return AdsRecoil;
}

UPxiiRecoilPattern* APxiiWeaponRange::GetHipRecoil()
{
	return HipRecoil;
}

TArray<UNiagaraSystem*> APxiiWeaponRange::GetSystem()
{
	return MuzzleFlashEffect;
}
