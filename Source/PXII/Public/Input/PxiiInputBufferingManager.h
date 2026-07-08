#pragma once
#include "PlayerInputManagerBase.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "PxiiInputBufferingManager.generated.h"

USTRUCT()
struct FBufferedInput
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag InputTag;

	UPROPERTY()
	float RemainingTime = 0.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPxiiInputBufferedDelegate, FGameplayTag, InputTag);

UCLASS()
class PXII_API UPxiiInputBufferingManager : public UPlayerInputManagerBase
{
	GENERATED_BODY()
	
public:

	FPxiiInputBufferedDelegate OnInputBuffered;

	void Tick(float DeltaTime);

	void BufferInput(const FGameplayTag& InputTag, float BufferTime = 0.25f);

	bool HasBufferedInput(const FGameplayTag& InputTag) const;

	bool HasQueuedInput(const FGameplayTag& InputTag) const;
	
	bool ConsumeBufferedInput(const FGameplayTag& InputTag);

	bool ConsumeAndQueueBufferedInput(const FGameplayTag& InputTag);
	
	FGameplayTag ConsumeQueuedInput();

	void Clear();

private:

	UPROPERTY()
	TArray<FBufferedInput> BufferedInputs;

	TArray<FBufferedInput> QueuedInputs;
};
