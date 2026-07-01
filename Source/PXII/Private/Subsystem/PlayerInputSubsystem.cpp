#include "Subsystem/PlayerInputSubsystem.h"

void UPlayerInputSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	MappingManager = NewObject<UPxiiInputMappingManager>(this);
	
	MappingManager->Initialize(GetLocalPlayer());
}

void UPlayerInputSubsystem::Deinitialize()
{
	Super::Deinitialize();
}
