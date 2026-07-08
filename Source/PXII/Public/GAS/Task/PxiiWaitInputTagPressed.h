#pragma once
#include "Abilities/Tasks/AbilityTask.h"
#include "GameplayTagContainer.h"
#include "Subsystem/PlayerInputSubsystem.h"
#include "PxiiWaitInputTagPressed.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPxiiInputTagPressedDelegate, FGameplayTag, InputTag);

UCLASS()
class PXII_API UPxiiWaitInputTagPressed : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FPxiiInputTagPressedDelegate OnPressed;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(DisplayName="Wait Input Tag Pressed",
		HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="TRUE"))
	static UPxiiWaitInputTagPressed* WaitInputTagPressed(UGameplayAbility* OwningAbility, FGameplayTag InputTag);

protected:
	
	UPROPERTY()
	TObjectPtr<UPlayerInputSubsystem> InputSubsystem;

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:

	UFUNCTION()
	void HandleInputPressed(FGameplayTag PressedTag);

	FGameplayTag ExpectedInputTag;

};
