#include "GAS/Task/PxiiWaitInputTagPressed.h"

#include "../../../Public/Utility/PXIILogUtility.h"

UPxiiWaitInputTagPressed* UPxiiWaitInputTagPressed::WaitInputTagPressed(
	UGameplayAbility* OwningAbility, FGameplayTag InputTag)
{
	auto* Task = NewAbilityTask<UPxiiWaitInputTagPressed>(OwningAbility);
	Task->ExpectedInputTag = InputTag;

	return Task;
}

void UPxiiWaitInputTagPressed::Activate()
{
	Super::Activate();
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();

	if (!PC)
	{
		EndTask();
		return;
	}

	ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();

	if (!LocalPlayer)
	{
		EndTask();
		return;
	}

	InputSubsystem = LocalPlayer->GetSubsystem<UPlayerInputSubsystem>();

	if (!InputSubsystem)
	{
		EndTask();
		return;
	}
	
	InputSubsystem->GetInputBufferManager()->OnInputBuffered.AddDynamic(this, &UPxiiWaitInputTagPressed::HandleInputPressed);
}

void UPxiiWaitInputTagPressed::OnDestroy(bool bInOwnerFinished)
{
	Super::OnDestroy(bInOwnerFinished);
}

void UPxiiWaitInputTagPressed::HandleInputPressed(FGameplayTag PressedTag)
{
	PXII_LOG(ELogCategory::Ability, Log, TEXT("DZ_LOG:: Tag Buffered Received! Tag : %s"), *PressedTag.ToString());
	if (PressedTag != ExpectedInputTag)
	{
		return;
	}
	
	PXII_LOG(ELogCategory::Ability, Log, TEXT("DZ_LOG:: Tag Buffered Matched! Tag : %s"), *PressedTag.ToString());
	InputSubsystem->GetInputBufferManager()->ConsumeBufferedInput(PressedTag);
	OnPressed.Broadcast();

	EndTask();
}