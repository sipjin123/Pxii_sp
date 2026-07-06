#include "Input/PxiiInputStateManager.h"
#include "Windows/WindowsPlatformTime.h"

void UPxiiInputStateManager::Tick(float DeltaTime)
{
	for (auto& Pair : InputStates)
	{
		FInputState& PlayerInput = Pair.Value;

		if (PlayerInput.InputState == EInputState::Pressed)
		{
			PlayerInput.InputState = EInputState::Held;
			PlayerInput.HeldTime += DeltaTime;
		}
		else if(PlayerInput.InputState == EInputState::Held)
		{
			PlayerInput.HeldTime += DeltaTime;
		}
		else if(PlayerInput.InputState == EInputState::Released)
		{
			PlayerInput.InputState = EInputState::None;
			PlayerInput.HeldTime = 0.0f;
		}
	}
}

void UPxiiInputStateManager::PressInput(const FGameplayTag& InputTag)
{
	FInputState& PlayerInput = InputStates.FindOrAdd(InputTag);
	
	PlayerInput.InputState = EInputState::Pressed;
	PlayerInput.HeldTime = 0.f;
	PlayerInput.LastPressedTime = FPlatformTime::Seconds();
}

void UPxiiInputStateManager::ReleaseInput(const FGameplayTag& InputTag)
{
	if (FInputState* PlayerInput = InputStates.Find(InputTag))
	{
		PlayerInput->InputState = EInputState::Released;
		PlayerInput->LastReleasedTime = FPlatformTime::Seconds();
	}
}

bool UPxiiInputStateManager::IsPressed(const FGameplayTag& InputTag) const
{
	return GetInputState(InputTag) == EInputState::Pressed;
}

bool UPxiiInputStateManager::IsHeld(const FGameplayTag& InputTag) const
{
	return GetInputState(InputTag) == EInputState::Held;
}

bool UPxiiInputStateManager::WasReleased(const FGameplayTag& InputTag) const
{
	return GetInputState(InputTag) == EInputState::Released;
}

EInputState UPxiiInputStateManager::GetInputState(const FGameplayTag& InputTag) const
{
	if (const FInputState* PlayerInput = FindState(InputTag))
	{
		return PlayerInput->InputState;
	}
	
	return EInputState::None;
}

float UPxiiInputStateManager::GetHeldTime(const FGameplayTag& InputTag) const
{
	if (const FInputState* PlayerInput = FindState(InputTag))
	{
		return PlayerInput->HeldTime;
	}

	return 0.f;
}

void UPxiiInputStateManager::ResetFrameState()
{
	for (auto& Pair : InputStates)
	{
		Pair.Value.InputState = EInputState::None;
	}
}

const FInputState* UPxiiInputStateManager::FindState(const FGameplayTag& InputTag) const
{
	return InputStates.Find(InputTag);
}