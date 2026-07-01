#include "Input/PlayerInputManagerBase.h"

UEnhancedInputLocalPlayerSubsystem* UPlayerInputManagerBase::GetInputSubsystem() const
{
	if (LocalPlayer)
	{
		return LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	}

	return nullptr;
}
