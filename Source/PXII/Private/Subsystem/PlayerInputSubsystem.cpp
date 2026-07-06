#include "Subsystem/PlayerInputSubsystem.h"

void UPlayerInputSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	MappingManager = NewObject<UPxiiInputMappingManager>(this);
	BufferManager = NewObject<UPxiiInputBufferingManager>(this);
	InputStateManager = NewObject<UPxiiInputStateManager>(this);
	AbilityRouterManager = NewObject<UPxiiAbilityRouterManager>(this);
	
	MappingManager->Initialize(GetLocalPlayer());
	BufferManager->Initialize(GetLocalPlayer());
	InputStateManager->Initialize(GetLocalPlayer());
	AbilityRouterManager->Initialize(GetLocalPlayer());
}

void UPlayerInputSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UPlayerInputSubsystem::InitializeForPawn(APawn* Pawn)
{
	if (!Pawn)
	{
		return;
	}

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Pawn))
	{
		if (ASI->GetAbilitySystemComponent())
		{
			GetAbilityRouterManager()->InitializeASC(ASI->GetAbilitySystemComponent());
		}
	}
}

UPxiiInputMappingManager* UPlayerInputSubsystem::GetInputMapManager()
{
	return MappingManager;
}

UPxiiInputBufferingManager* UPlayerInputSubsystem::GetInputBufferManager()
{
	return BufferManager;
}

UPxiiInputStateManager* UPlayerInputSubsystem::GetInputStateManager()
{
	return InputStateManager;
}

UPxiiAbilityRouterManager* UPlayerInputSubsystem::GetAbilityRouterManager()
{
	return AbilityRouterManager;
}
