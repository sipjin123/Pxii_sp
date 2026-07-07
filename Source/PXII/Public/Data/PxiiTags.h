// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Damage_Crit);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Damage_Fire);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Damage_Ice);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_HitType);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DamageSource);

namespace Projectiles
{
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_Projectiles_Basic);
	//Extend more projectile tags here
	//Using Projectiles namespace for better organization and to avoid name conflicts
}

namespace UI 
{
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_UI_WidgetStack_HUD);
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_UI_WidgetStack_Menu);
}