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

UCLASS()
class PXII_API UPxiiInputBufferingManager : public UPlayerInputManagerBase
{
	GENERATED_BODY()
	
public:

	void Tick(float DeltaTime);

	void BufferInput(const FGameplayTag& InputTag, float BufferTime = 0.25f);

	bool HasBufferedInput(const FGameplayTag& InputTag) const;

	bool ConsumeBufferedInput(const FGameplayTag& InputTag);

	void Clear();

private:

	UPROPERTY()
	TArray<FBufferedInput> BufferedInputs;
};
