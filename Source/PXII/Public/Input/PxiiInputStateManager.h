#pragma once
#include "PlayerInputManagerBase.h"
#include "PxiiInputStateManager.generated.h"

UENUM()
enum class EInputState : uint8
{
	None,
	Pressed,
	Held,
	Released
};

USTRUCT(BlueprintType)
struct FInputState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	EInputState InputState;
	
	UPROPERTY(VisibleAnywhere)
	float HeldTime = 0.f;

	UPROPERTY(VisibleAnywhere)
	float LastPressedTime = 0.f;

	UPROPERTY(VisibleAnywhere)
	float LastReleasedTime = 0.f;
};

UCLASS()
class PXII_API UPxiiInputStateManager : public UPlayerInputManagerBase
{
	GENERATED_BODY()

public:

	void Tick(float DeltaTime);

	void PressInput(const FGameplayTag& InputTag);
	void ReleaseInput(const FGameplayTag& InputTag);

	bool IsPressed(const FGameplayTag& InputTag) const;
	bool IsHeld(const FGameplayTag& InputTag) const;
	bool WasReleased(const FGameplayTag& InputTag) const;
	EInputState GetInputState(const FGameplayTag& InputTag) const;

	float GetHeldTime(const FGameplayTag& InputTag) const;

	void ResetFrameState();

private:

	UPROPERTY()
	TMap<FGameplayTag, FInputState> InputStates;
	
	const FInputState* FindState(const FGameplayTag& InputTag) const;
};
