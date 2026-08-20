#include "PxiiCustomizationUIBase.h"
#include "ICommonInputModule.h"
#include "Input/CommonUIInputTypes.h"

void UPxiiCustomizationUIBase::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UPxiiCustomizationUIBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
		)
	);
}

void UPxiiCustomizationUIBase::OnBackBoundActionTriggered_Implementation()
{
	DeactivateWidget();
}
