#include "Input/PxiiInputBufferingManager.h"

void UPxiiInputBufferingManager::Tick(float DeltaTime)
{
	for (int32 index = BufferedInputs.Num() - 1; index >= 0; --index)
	{
		BufferedInputs[index].RemainingTime -= DeltaTime;

		if (BufferedInputs[index].RemainingTime <= 0.f)
		{
			BufferedInputs.RemoveAt(index);
		}
	}
}

void UPxiiInputBufferingManager::BufferInput(const FGameplayTag& InputTag, float BufferTime)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (FBufferedInput& Input : BufferedInputs)
	{
		if (Input.InputTag == InputTag)
		{
			Input.RemainingTime = BufferTime;
			return;
		}
	}

	FBufferedInput NewInput;
	NewInput.InputTag = InputTag;
	NewInput.RemainingTime = BufferTime;

	BufferedInputs.Add(NewInput);
}

bool UPxiiInputBufferingManager::HasBufferedInput(const FGameplayTag& InputTag) const
{
	return BufferedInputs.ContainsByPredicate([&InputTag](const FBufferedInput& Input)
	{
		return Input.InputTag == InputTag;
	});
}

bool UPxiiInputBufferingManager::ConsumeBufferedInput(const FGameplayTag& InputTag)
{
	const int32 Index = BufferedInputs.IndexOfByPredicate([&InputTag](const FBufferedInput& Input)
	{
		return Input.InputTag == InputTag;
	});

	if (Index == INDEX_NONE)
	{
		return false;
	}

	BufferedInputs.RemoveAt(Index);
	return true;
}

void UPxiiInputBufferingManager::Clear()
{
	BufferedInputs.Empty();
}
