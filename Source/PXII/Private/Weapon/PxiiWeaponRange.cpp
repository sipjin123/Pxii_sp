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
		for(TPair<FName, FMuzzleEffect> system : MuzzleFlashEffect)
		{
			TArray<UNiagaraSystem*> effects = system.Value.MuzzleEffect;
			if(effects.IsEmpty())
			{
				continue;
			}
			FMuzzleEffectComp comp;
			for(UNiagaraSystem* effect : effects)
			{
				if(!effect)
				{
					continue;
				}

				FString vfxName = "MuzzleFlash-" + FString::FromInt(index);
				UNiagaraComponent* MuzzleFlashComponent = NewObject<UNiagaraComponent>(this, FName(*vfxName));
				MuzzleFlashComponent->AttachToComponent(SKWeapon, FAttachmentTransformRules::SnapToTargetIncludingScale, MuzzleSocketName);

				MuzzleFlashComponent->SetupAttachment(SKWeapon, MuzzleSocketName);
				MuzzleFlashComponent->RegisterComponent();

				MuzzleFlashComponent->SetAutoActivate(false);
				MuzzleFlashComponent->SetAsset(effect);
				MuzzleFlashComponent->Deactivate();
				HasMuzzleFlash = true;

				index++;
				comp.MuzzleEffectComp.Add(MuzzleFlashComponent);
			}
			
			MuzzleFlashComponents.Add(system.Key, comp);
		}
	}
	else
	{
		HasMuzzleFlash = false;
	}
}

void APxiiWeaponRange::PlayMuzzleFlash_Implementation(FName MuzzleName)
{
	if(!HasMuzzleFlash)
	{
		return;
	}

	if(!MuzzleFlashComponents.Contains(MuzzleName))
	{
		return;
	}
	FMuzzleEffectComp comps = MuzzleFlashComponents[MuzzleName];
	for(UNiagaraComponent* comp : comps.MuzzleEffectComp)
	{
		comp->Activate(true);
	}
}

void APxiiWeaponRange::StopMuzzleFlash_Implementation(FName MuzzleName)
{
	if(!HasMuzzleFlash)
	{
		return;
	}

	if(!MuzzleFlashComponents.Contains(MuzzleName))
	{
		return;
	}
	FMuzzleEffectComp comps = MuzzleFlashComponents[MuzzleName];
	for(UNiagaraComponent* comp : comps.MuzzleEffectComp)
	{
		comp->Deactivate();
	}
}

void APxiiWeaponRange::ResetVent_Implementation()
{
	
}

float APxiiWeaponRange::GetDamage(float hitDistance, float bulletBonusDamage, bool isCritical)
{
	if(BlasterData == nullptr)
	{
		return 1.0f;
	}
	
	float critMult = isCritical ? BlasterData->CriticalMultiplier : 1.0f;
	if(BlasterData->EnableDistanceFalloff)
	{
		float startDist = FMath::Min(BlasterData->DistanceFalloffStart, BlasterData->DistanceFalloffEnd);
		float endDist = FMath::Max(BlasterData->DistanceFalloffStart, BlasterData->DistanceFalloffEnd);
		float fallOffDistance = (hitDistance - startDist) / FMath::Max(endDist - startDist, 1);
		float alpha = FMath::Clamp(fallOffDistance, 0, 1);
		float Damage = FMath::Lerp(BlasterData->MinimumDamage + bulletBonusDamage, BlasterData->BaseDamage + bulletBonusDamage, 1.0f - alpha);
		return Damage * critMult; 
	}

	return (BlasterData->BaseDamage + bulletBonusDamage) * critMult;
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

// TArray<UNiagaraSystem*> APxiiWeaponRange::GetSystem()
// {
// 	return MuzzleFlashEffect;
// }
