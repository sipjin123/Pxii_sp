#include "CommonUI\PxiiActivatableWidget.h"

void UPxiiActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
}

void UPxiiActivatableWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
}

APxiiPlayerController* UPxiiActivatableWidget::GetPlayerController() const
{
	return Cast<APxiiPlayerController>(GetOwningPlayer());
}

APxiiCharacter* UPxiiActivatableWidget::GetPlayerPawn() const
{
	return Cast<APxiiCharacter>(GetOwningPlayerPawn());
}
