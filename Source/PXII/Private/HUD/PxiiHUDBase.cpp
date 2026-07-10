#include "HUD/PxiiHUDBase.h"
#include "Subsystem/PxiiUISubsystem.h"
#include "Utility/PXIILogUtility.h"
#include "CommonUI/PxiiActivatableWidget.h"

void APxiiHUDBase::BeginPlay()
{
	Super::BeginPlay();
	
	CacheReferences();

	if(!StackLayoutClass)
	{
		PXII_LOG(ELogCategory::UI, Error, TEXT("[%s]: StackLayoutClass is not assigned."), *ThisClass::StaticClass()->GetName());
		return;
	}

	StackLayout = CreateWidget<UPxiiWidgetStackBase>(GetOwningPlayerController(), StackLayoutClass);

	if(StackLayout)
	{
		StackLayout->AddToViewport();
		UISubsystem->RegisterBaseStack(StackLayout);
	}
}

void APxiiHUDBase::CacheReferences()
{
	PlayerController = Cast<APxiiPlayerController>(GetOwningPlayerController());

	if (PlayerController)
	{
		PlayerCharacter = Cast<APxiiCharacter>(PlayerController->GetPawn());
	}

	if(PlayerCharacter)
	{
		UISubsystem = PlayerCharacter->GetGameInstance()->GetSubsystem<UPxiiUISubsystem>();

		if(!UISubsystem)
		{
			PXII_LOG(ELogCategory::UI, Error, TEXT("[%s]: UISubsystem is not found."), *ThisClass::StaticClass()->GetName());
		}
	}
}
