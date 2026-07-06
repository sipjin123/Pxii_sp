#pragma once
#include "Input/PxiiPlayerInputConfig.h"
#include "Subsystem/PlayerInputSubsystem.h"
#include "PxiiPlayerController.generated.h"

UCLASS(Blueprintable)
class PXII_API APxiiPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UPxiiPlayerInputConfig> InputConfig;

private:

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagHolding(FGameplayTag InputTag);
	
	UPlayerInputSubsystem* GetPlayerInputSubsystem() const;

};


