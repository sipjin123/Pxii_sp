#include "PlayerController/PxiiPlayerController.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemGlobals.h"
#include "GAS/PxiiAbilitySystemComponent.h"
#include "Input/PxiiPlayerInputComponent.h"
#include "Utility/PXIILogUtility.h"

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
		PXII_LOG(ELogCategory::Controls, Error, TEXT("DZ_LOG:: Input is NULL"));
	}
	
	if (UPxiiPlayerInputComponent* EnhancedInputComponent = Cast<UPxiiPlayerInputComponent>(InputComponent))
	{
		PXII_LOG(ELogCategory::Controls, Log, TEXT("DZ_LOG:: Binding Input"));

		// Moving
		EnhancedInputComponent->BindAction(MoveInput, ETriggerEvent::Triggered, this, &APxiiPlayerController::Move);
		// Looking
		EnhancedInputComponent->BindAction(LookInput, ETriggerEvent::Triggered, this, &APxiiPlayerController::Look);

		EnhancedInputComponent->BindAction(MoveInput, ETriggerEvent::Completed, this, &APxiiPlayerController::Move);
		// Looking
		EnhancedInputComponent->BindAction(LookInput, ETriggerEvent::Completed, this, &APxiiPlayerController::Look);
		
		EnhancedInputComponent->BindAbilityActions(InputConfig, this, &APxiiPlayerController::AbilityInputTagPressed, &APxiiPlayerController::AbilityInputTagReleased);
	}
}

void APxiiPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (UPlayerInputSubsystem* InputSubsystem =	GetLocalPlayer()->GetSubsystem<UPlayerInputSubsystem>())
	{
		InputSubsystem->InitializeForPawn(InPawn);
	}

	ASC = Cast<UPxiiAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InPawn));
}

void APxiiPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (IsMovementBlocked) return;
	APawn* TargetPawn = GetPawn().Get();

	if (!TargetPawn)
	{
		return;
	}

	CachedMovementInput = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(ASC && ASC->IsInputBlocked())
	{
		CachedMovementInput = FVector2D::ZeroVector;
		PXII_LOG(ELogCategory::Controls, Error, TEXT("DZ_LOG:: Input Blocked"));
		return;
	}
	
	if (TargetPawn || !ASC->IsInputBlocked())
	{
		if (CachedMovementInput.Y > 0)
		{
			// Block forward movement
		}
		else
		{
			//ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		}

		PXII_LOG(ELogCategory::Controls, Log, TEXT("DZ_LOG:: [Move] Pawn: %s Input control: %s"), *TargetPawn->GetClass()->GetName(), *CachedMovementInput.ToString());

		TargetPawn->AddMovementInput(ForwardDirection, CachedMovementInput.Y);
		TargetPawn->AddMovementInput(RightDirection, CachedMovementInput.X);
		//MoveCharacter(LastMovementInput, ForwardDirection, RightDirection);
	}
	else
	{
		PXII_LOG(ELogCategory::Controls, Error, TEXT("DZ_LOG:: [Move] Pawn is Null"));
	}
}

void APxiiPlayerController::Look(const FInputActionValue& InputActionValue)
{
	if (IsAimBlocked) return;
	APawn* TargetPawn = GetPawn().Get();
	if (!TargetPawn)
	{
		PXII_LOG(ELogCategory::Controls, Error, TEXT("DZ_LOG:: [Look] Pawn is Null"));
		return;
	}
	
	CachedLookInput = InputActionValue.Get<FVector2D>();
	
	PXII_LOG(ELogCategory::Controls, Log, TEXT("DZ_LOG:: [Look] Input control: %s"), *CachedLookInput.ToString());

	TargetPawn->AddControllerYawInput(-CachedLookInput.X * AimYawScale);
	TargetPawn->AddControllerPitchInput(CachedLookInput.Y * AimPitchScale);
}

FVector2D APxiiPlayerController::GetCachedLookInput() const
{
	return CachedLookInput;
}

FVector2D APxiiPlayerController::GetCacheMoveInput() const
{
	return CachedMovementInput;
}

void APxiiPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	UPlayerInputSubsystem* InputSubsystem = GetPlayerInputSubsystem();

	if (!InputSubsystem)
	{
		return;
	}

	InputSubsystem->GetInputStateManager()->PressInput(InputTag);

	PXII_LOG(ELogCategory::Controls, Log, TEXT("DZ_LOG:: INPUT Pressed! Tag: %s"), *InputTag.ToString());

	if(!InputSubsystem->GetAbilityRouterManager()->HandleInputPressed(InputTag))
	{
		InputSubsystem->GetInputBufferManager()->BufferInput(InputTag);
		UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: Input Buffered"))
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: Input Consumed"));
	}
	
}

void APxiiPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	UPlayerInputSubsystem* InputSubsystem = GetPlayerInputSubsystem();

	if (!InputSubsystem)
	{
		return;
	}
	
	PXII_LOG(ELogCategory::Controls, Log, TEXT("DZ_LOG:: INPUT COMPLETED! Tag: %s"), *InputTag.ToString());
	InputSubsystem->GetInputStateManager()->ReleaseInput(InputTag);
	InputSubsystem->GetAbilityRouterManager()->HandleInputReleased(InputTag);
}

UPlayerInputSubsystem* APxiiPlayerController::GetPlayerInputSubsystem() const
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		return LocalPlayer->GetSubsystem<UPlayerInputSubsystem>();
	}

	return nullptr;
}
