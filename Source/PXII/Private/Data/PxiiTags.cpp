// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/PxiiTags.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Crit, "Combat.Damage.Crit");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Fire, "Combat.Damage.Fire");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Ice,  "Combat.Damage.Ice");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Lightning, "Combat.Damage.Lightning");

UE_DEFINE_GAMEPLAY_TAG(TAG_HitType,  "Combat.HitType");
UE_DEFINE_GAMEPLAY_TAG(TAG_DamageSource,  "Combat.DamageSource")
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_YinYangType, "Stat.YinYang");
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_Amount, "Stat.Amount");
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_IsPercent, "Data.IsPercent");

namespace Projectiles
{
	UE_DEFINE_GAMEPLAY_TAG(Pxii_Projectiles_Basic, "Pxii.Projectiles.Basic");
}

namespace UI
{
	// Widget Stack
	UE_DEFINE_GAMEPLAY_TAG(Pxii_UI_WidgetStack_Frontend, "Pxii.UI.WidgetStack.Frontend");
	UE_DEFINE_GAMEPLAY_TAG(Pxii_UI_WidgetStack_HUD, "Pxii.UI.WidgetStack.HUD");
	UE_DEFINE_GAMEPLAY_TAG(Pxii_UI_WidgetStack_Menu, "Pxii.UI.WidgetStack.Menu");
	UE_DEFINE_GAMEPLAY_TAG(Pxii_UI_WidgetStack_Modal, "Pxii.UI.WidgetStack.Modal");

	// Widgets
	UE_DEFINE_GAMEPLAY_TAG(Pxii_UI_Widget_HUD, "Pxii.UI.Widget.HUD");
	UE_DEFINE_GAMEPLAY_TAG(Pxii_UI_Widget_CheatMenu, "Pxii.UI.Widget.CheatMenu");
	UE_DEFINE_GAMEPLAY_TAG(Pxii_UI_Widget_TitleScreen_Start, "Pxii.UI.Widget.TitleScreen.Start");
	UE_DEFINE_GAMEPLAY_TAG(Pxii_UI_Widget_TitleScreen_Main, "Pxii.UI.Widget.TitleScreen.Main");
}