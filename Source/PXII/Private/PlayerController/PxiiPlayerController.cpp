#include "PlayerController/PxiiPlayerController.h"

#include "Input/PxiiIPlayerInputComponent.h"

void APxiiPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UPlayerInputSubsystem* InputSubsystem = GetPlayerInputSubsystem())
	{
		InputSubsystem->GetInputMapManager()->AddMappingContext(DefaultMappingContext, 0);
	}
}

void APxiiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(!InputComponent)
	{
		UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: Input is NULL"));
	}
	
	if (UPxiiIPlayerInputComponent* EnhancedInputComponent = Cast<UPxiiIPlayerInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAbilityActions(InputConfig, this, &APxiiPlayerController::AbilityInputTagPressed, &APxiiPlayerController::AbilityInputTagReleased, &APxiiPlayerController::AbilityInputTagHeld, &APxiiPlayerController::AbilityInputTagHolding);
	}
}

void APxiiPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (UPlayerInputSubsystem* InputSubsystem =	GetLocalPlayer()->GetSubsystem<UPlayerInputSubsystem>())
	{
		InputSubsystem->InitializeForPawn(InPawn);
	}

}

void APxiiPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	UPlayerInputSubsystem* InputSubsystem = GetPlayerInputSubsystem();

	if (!InputSubsystem)
	{
		return;
	}

	InputSubsystem->GetInputStateManager()->PressInput(InputTag);
	InputSubsystem->GetInputBufferManager()->BufferInput(InputTag);

	if(InputSubsystem->GetAbilityRouterManager()->HandleInputPressed(InputTag))
	{
		UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: Input Consumed"));
		InputSubsystem->GetInputBufferManager()->ConsumeBufferedInput(InputTag);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: Input Buffered"))
	}
	
}

void APxiiPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	UPlayerInputSubsystem* InputSubsystem = GetPlayerInputSubsystem();

	if (!InputSubsystem)
	{
		return;
	}

	InputSubsystem->GetInputStateManager()->ReleaseInput(InputTag);
	InputSubsystem->GetAbilityRouterManager()->HandleInputReleased(InputTag);
}

void APxiiPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	
}

void APxiiPlayerController::AbilityInputTagHolding(FGameplayTag InputTag)
{
	
}

UPlayerInputSubsystem* APxiiPlayerController::GetPlayerInputSubsystem() const
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		return LocalPlayer->GetSubsystem<UPlayerInputSubsystem>();
	}

	return nullptr;
}
