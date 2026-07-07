#include "CommonUI/PxiiCommonUserWidget.h"

void UPxiiCommonUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPxiiCommonUserWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UPxiiCommonUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

APxiiPlayerController* UPxiiCommonUserWidget::GetPlayerController() const
{
	return Cast<APxiiPlayerController>(GetOwningPlayer());
}

APxiiCharacter* UPxiiCommonUserWidget::GetPlayerPawn() const
{
	return Cast<APxiiCharacter>(GetOwningPlayerPawn());
}


