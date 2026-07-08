// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/PxiiCombatBPLibrary.h"
#include "Settings/CombatDeveloperSettings.h"
#include "Utility/PXIILogUtility.h"

TSoftClassPtr<APxiiProjectileBase> UPxiiCombatBPLibrary::GetSoftProjectileClassByTag(UPARAM(meta = (Categories = "Pxii.Projectiles")) FGameplayTag InTag)
{
	const UCombatDeveloperSettings* ProjectileDevSettings = GetDefault<UCombatDeveloperSettings>();

	if(!ProjectileDevSettings->ProjectileClassesMap.Contains(InTag))
	{
		PXII_LOG(ELogCategory::Combat, Warning, TEXT("[%s]: Projectile class %s is not assigned in Developer Settings"), *ThisClass::StaticClass()->GetName(), *InTag.ToString());
		return nullptr;
	}

	return ProjectileDevSettings->ProjectileClassesMap.FindRef(InTag);
}