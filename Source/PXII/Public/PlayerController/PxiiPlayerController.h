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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> MoveInput;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> LookInput;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	virtual void Move(const FInputActionValue& InputActionValue);
	virtual void Look(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintPure)
	FVector2D GetCacheMoveInput() const;

	UFUNCTION(BlueprintPure)
	FVector2D GetCachedLookInput() const;
	
	UPROPERTY(BlueprintReadWrite)
	bool IsMovementBlocked;
	UPROPERTY(BlueprintReadWrite)
	bool IsAimBlocked;
	
	UPROPERTY(BlueprintReadWrite)
	FVector2D CachedMovementInput;
	
	UPROPERTY(BlueprintReadWrite)
	FVector2D CachedLookInput;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UPxiiPlayerInputConfig> InputConfig;

	// Aim Pitch Scale
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float  AimPitchScale = 0.25f;

	// Aim Yaw Scale
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float  AimYawScale = 0.25f;

private:

	UPROPERTY()
	TObjectPtr<UPxiiAbilitySystemComponent> ASC;
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagHolding(FGameplayTag InputTag);
	
	UPlayerInputSubsystem* GetPlayerInputSubsystem() const;

};


