#include "Input/PxiiInputMappingManager.h"

bool UPxiiInputMappingManager::AddMappingContext(const UInputMappingContext* MappingContext, int32 Priority)
{
	if (!MappingContext)
	{
		return false;
	}

	if (HasMappingContext(MappingContext))
	{
		return false;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetInputSubsystem())
	{
		Subsystem->AddMappingContext(const_cast<UInputMappingContext*>(MappingContext), Priority);

		FMappingContextEntry MapEntry;
		MapEntry.MappingContext = MappingContext;
		MapEntry.Priority = Priority;
		
		ActiveMappings.Add(MapEntry);
		return true;
	}

	return false;
}

bool UPxiiInputMappingManager::RemoveMappingContext(const UInputMappingContext* MappingContext)
{
	if (!MappingContext)
	{
		return false;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetInputSubsystem())
	{
		Subsystem->RemoveMappingContext(MappingContext);

		ActiveMappings.RemoveAll([MappingContext](const FMappingContextEntry& Entry)
		{
			return Entry.MappingContext == MappingContext;
		});

		return true;
	}

	return false;
}

void UPxiiInputMappingManager::ClearAllMappings()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetInputSubsystem())
	{
		Subsystem->ClearAllMappings();
	}

	ActiveMappings.Empty();
}

bool UPxiiInputMappingManager::HasMappingContext(
	const UInputMappingContext* MappingContext) const
{
	return ActiveMappings.ContainsByPredicate([MappingContext](const FMappingContextEntry& Entry)
	{
		return Entry.MappingContext == MappingContext;
	});
}
