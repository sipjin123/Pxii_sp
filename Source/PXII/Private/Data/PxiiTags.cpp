// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/PxiiTags.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Crit, "Damage.Crit");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Fire, "Damage.Fire");
UE_DEFINE_GAMEPLAY_TAG(TAG_Damage_Ice,  "Damage.Ice");
UE_DEFINE_GAMEPLAY_TAG(TAG_HitType,  "Combat.HitType");
UE_DEFINE_GAMEPLAY_TAG(TAG_DamageSource,  "Combat.DamageSource")

namespace Projectiles
{
	UE_DEFINE_GAMEPLAY_TAG(Pxii_Projectiles_Basic, "Pxii.Projectiles.Basic");
}

namespace UI
{
	UE_DEFINE_GAMEPLAY_TAG(Pxii_UI_WidgetStack_Frontend, "Pxii.UI.WidgetStack.Frontend");
	UE_DEFINE_GAMEPLAY_TAG(Pxii_UI_WidgetStack_HUD, "Pxii.UI.WidgetStack.HUD");
	UE_DEFINE_GAMEPLAY_TAG(Pxii_UI_WidgetStack_Menu, "Pxii.UI.WidgetStack.Menu");
	UE_DEFINE_GAMEPLAY_TAG(Pxii_UI_WidgetStack_Modal, "Pxii.UI.WidgetStack.Modal");
}