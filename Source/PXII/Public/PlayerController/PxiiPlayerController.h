#pragma once
#include "Input/PxiiPlayerInputConfig.h"
#include "Math/Vector2D.h"
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

	virtual void Move(const FInputActionValue& InputActionValue);
	virtual void Look(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintPure)
	FVector2D GetLastMovementInput() const;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UPxiiPlayerInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> MoveInput;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> LookInput;

	UPROPERTY()
	FVector2D LastMovementInput;

	// Aim Pitch Scale
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float  AimPitchScale = 0.25f;

	// Aim Yaw Scale
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float  AimYawScale = 0.25f;
	
private:

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagHolding(FGameplayTag InputTag);
	
	UPlayerInputSubsystem* GetPlayerInputSubsystem() const;

};


