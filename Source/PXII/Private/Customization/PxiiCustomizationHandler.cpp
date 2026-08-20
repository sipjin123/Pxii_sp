#include "Customization/PxiiCustomizationHandler.h"

void APxiiCustomizationHandler::OpenCustomization_Implementation()
{
	CustomizationActive = true;
}


void APxiiCustomizationHandler::CloseCustomization_Implementation()
{
	CustomizationActive = false;
}

bool APxiiCustomizationHandler::GetCustomizationActive()
{
	return CustomizationActive;
}
