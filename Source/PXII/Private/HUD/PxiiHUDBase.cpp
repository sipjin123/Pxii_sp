#include "HUD/PxiiHUDBase.h"

void APxiiHUDBase::BeginPlay()
{
	Super::BeginPlay();
	
	CacheReferences();
	CreateHUDLayout();
}

void APxiiHUDBase::CacheReferences()
{
	PlayerController = Cast<APxiiPlayerController>(GetOwningPlayerController());

	if (PlayerController)
	{
		PlayerCharacter = Cast<APxiiCharacter>(PlayerController->GetPawn());
	}
}

void APxiiHUDBase::CreateHUDLayout()
{
	if (!HUDLayoutClass || !PlayerController)
	{
		return;
	}

	HUDLayout = CreateWidget<UPxiiHUD>(PlayerController, HUDLayoutClass);

	if (HUDLayout)
	{
		HUDLayout->AddToViewport();
	}
}
