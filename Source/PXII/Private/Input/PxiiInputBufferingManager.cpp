#include "Input/PxiiInputBufferingManager.h"

#include "Utility/PXIILogUtility.h"

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

	if(HasQueuedInput(InputTag))
	{
		PXII_LOG(ELogCategory::Controls, Log, TEXT("Input Tag is Queued! Buffering of Input tag blocked!"));
		return;
	}

	for (FBufferedInput& Input : BufferedInputs)
	{
		if (Input.InputTag == InputTag)
		{
			Input.RemainingTime = BufferTime;
			OnInputBuffered.Broadcast(InputTag);
			return;
		}
	}

	FBufferedInput NewInput;
	NewInput.InputTag = InputTag;
	NewInput.RemainingTime = BufferTime;

	BufferedInputs.Add(NewInput);
	PXII_LOG(ELogCategory::Controls, Log, TEXT("DZ_LOG:: Input Buffered %s"), *InputTag.ToString());
	
	OnInputBuffered.Broadcast(InputTag);
}

bool UPxiiInputBufferingManager::HasBufferedInput(const FGameplayTag& InputTag) const
{
	return BufferedInputs.ContainsByPredicate([&InputTag](const FBufferedInput& Input)
	{
		return Input.InputTag == InputTag;
	});
}

bool UPxiiInputBufferingManager::HasQueuedInput(const FGameplayTag& InputTag) const
{
	return QueuedInputs.ContainsByPredicate([&InputTag](const FBufferedInput& Input)
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

bool UPxiiInputBufferingManager::ConsumeAndQueueBufferedInput(const FGameplayTag& InputTag)
{
	if(HasQueuedInput(InputTag))
	{
		PXII_LOG(ELogCategory::Ability, Log, TEXT("DZ_LOG:: Consume Failed!"));
		return false;
	}
	
	if(!ConsumeBufferedInput(InputTag))
	{
		return false;
	}
	
	FBufferedInput QueuedInput;
	QueuedInput.InputTag = InputTag;
	QueuedInputs.Add(QueuedInput);

	return true;
}

FGameplayTag UPxiiInputBufferingManager::ConsumeQueuedInput()
{
	if(QueuedInputs.IsEmpty())
	{
		return FGameplayTag::EmptyTag;
	}

	FGameplayTag Tag = QueuedInputs[0].InputTag;

	QueuedInputs.RemoveAt(0);
	return Tag;
}

void UPxiiInputBufferingManager::Clear()
{
	BufferedInputs.Empty();
}
