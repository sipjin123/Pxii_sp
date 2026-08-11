// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Damage_Crit);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Damage_Fire);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Damage_Ice);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Damage_Lightning);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_HitType);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_DamageSource);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_YinYangType);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_Amount);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_IsPercent);

namespace Projectiles
{
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_Projectiles_Basic);
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_Projectiles_AOE);
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_Projectiles_DPS);

	//Extend more projectile tags here
	//Using Projectiles namespace for better organization and to avoid name conflicts
}

namespace UI 
{
	// Widget Stack
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_UI_WidgetStack_Frontend);
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_UI_WidgetStack_HUD);
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_UI_WidgetStack_Menu);
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_UI_WidgetStack_Modal);

	// Widgets
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_UI_Widget_HUD);
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_UI_Widget_CheatMenu);
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_UI_Widget_TitleScreen_Start);
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_UI_Widget_TitleScreen_Main);
}

namespace GameLevels
{
	// Game Levels
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_Level_Title);
	PXII_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Pxii_Level_DevGym);
}
