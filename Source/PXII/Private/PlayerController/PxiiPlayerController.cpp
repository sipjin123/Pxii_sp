#include "PlayerController/PxiiPlayerController.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemGlobals.h"
#include "CommonInputSubsystem.h"
#include "Components/PxiiAimAssistComponent.h"
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

	CommonInput = UCommonInputSubsystem::Get(GetLocalPlayer());
	CommonInput->OnInputMethodChangedNative.AddUObject(this, &APxiiPlayerController::OnInputMethodChanged);
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
	AimAssistComp = InPawn->GetComponentByClass<UPxiiAimAssistComponent>();
	AimComp = InPawn->GetComponentByClass<UPxiiAimComponent>();
}

void APxiiPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (IsMovementBlocked)
	{
		CachedMovementInput = FVector2D::ZeroVector;
		return;
	}
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
	FVector2D screenPos = GetViewportCenter();
	CachedSlowdownFactor = AimComp->GetIsADSActive() ? AimAssistComp->GetAimSlowdownFactor(screenPos) : 1.0f;

	FVector2D modLookDelta = CachedLookInput * CachedSlowdownFactor;
	
	PXII_LOG(ELogCategory::Controls, Log, TEXT("DZ_LOG:: [Look] Input control: %s"), *CachedLookInput.ToString());

	TargetPawn->AddControllerYawInput(-modLookDelta.X * AimYawScale);
	TargetPawn->AddControllerPitchInput(modLookDelta.Y * AimPitchScale);
}

FVector2D APxiiPlayerController::GetCachedLookInput() const
{
	return CachedLookInput;
}

FVector2D APxiiPlayerController::GetViewportCenter() const
{
	int32 sizeX = 0;
	int32 sizeY = 0;

	GetViewportSize(sizeX, sizeY);

	return FVector2D(sizeX * 0.5f, sizeY * 0.5f);
}

float APxiiPlayerController::GetCachedSlowdownFactor() const
{
	return CachedSlowdownFactor;
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
		//UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: Input Buffered"))
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: Input Consumed"));
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

void APxiiPlayerController::OnInputMethodChanged(ECommonInputType inputType)
{
	FName NewGamepadName = CommonInput->GetCurrentGamepadName();
	
	if (UPlayerInputSubsystem* InputSubsystem = GetPlayerInputSubsystem())
	{
		InputSubsystem->GetInputMapManager()->ClearAllMappings();
		InputSubsystem->GetInputMapManager()->AddMappingContext(MappingContext[inputType], 0);

		bool isUpdated = InputSubsystem->GetInputMapManager()->HasMappingContext(MappingContext[inputType]);
		PXII_LOG(ELogCategory::Controls, Warning, TEXT("Input method changed: Mapping %s Success: %s"), *UEnum::GetValueAsString(inputType),
			isUpdated ? TEXT("TRUE") : TEXT("FALSE"));
	}
}

UPlayerInputSubsystem* APxiiPlayerController::GetPlayerInputSubsystem() const
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		return LocalPlayer->GetSubsystem<UPlayerInputSubsystem>();
	}

	return nullptr;
}