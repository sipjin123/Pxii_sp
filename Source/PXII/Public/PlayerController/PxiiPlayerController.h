#pragma once
#include "CommonButtonBase.h"
#include "Components/PxiiAimAssistComponent.h"
#include "Components/PxiiLedgeTraversal.h"

#include "GameFramework/PlayerController.h"
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
	TMap<ECommonInputType, TObjectPtr<UInputMappingContext>> MappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> MoveInput;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> LookInput;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> JumpInput;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	virtual void Move(const FInputActionValue& InputActionValue);
	virtual void Look(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintPure)
	FVector2D GetCacheMoveInput() const;

	UFUNCTION(BlueprintPure)
	FVector2D GetCachedLookInput() const;

	UFUNCTION(BlueprintPure)
	FVector2D GetViewportCenter() const;

	UFUNCTION(BlueprintPure)
	float GetCachedSlowdownFactor() const;
	
	UPROPERTY(BlueprintReadWrite)
	bool IsMovementBlocked;
	UPROPERTY(BlueprintReadWrite)
	bool IsAimBlocked;

	UPROPERTY(BlueprintReadWrite)
	FVector2D LedgeInput;
	
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
	
	// Invert mouse look
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	bool bInvertMouseX = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	bool bInvertMouseY = false;
	
	// Clamp pitch angle
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float PitchMin = -20.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float PitchMax = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	bool EnabledLedgeTraversal = false;

	UPROPERTY()
	float CachedSlowdownFactor;

	UFUNCTION()
	void OnInputMethodChanged(ECommonInputType inputType);

	UFUNCTION()
	bool OnGrabLedgeInput();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SwitchMappingControls(int32 mapIndex = 1);
	void SwitchMappingControls_Implementation(int32 mapIndex = 1);

	UFUNCTION(BlueprintNativeEvent)
	void OnControlMappingUpdate();
	void OnControlMappingUpdate_Implementation();

	UFUNCTION(BlueprintPure)
	FString GetActiveMapDisplayName();

	UFUNCTION()
	void OnJumpStarted();
	
	UFUNCTION()
	void OnJumpTriggered();
private:

	UPROPERTY()
	TObjectPtr<UPxiiAbilitySystemComponent> ASC;
	UPROPERTY()
	TObjectPtr<UPxiiAimAssistComponent> AimAssistComp;
	UPROPERTY()
	TObjectPtr<UPxiiAimComponent> AimComp;
	UPROPERTY()
	TObjectPtr<UPxiiLedgeTraversal> LedgeComp;
	UPROPERTY()
	TObjectPtr<UCommonInputSubsystem> CommonInput;
	UPROPERTY()
	ECommonInputType CurrentInputType = ECommonInputType::MouseAndKeyboard;
	UPROPERTY()
	int32 CurrentKeyboardMap = 0;
	UPROPERTY()
	FString ActiveMapContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UInputMappingContext>> KeyboardMapContext;
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagHolding(FGameplayTag InputTag);
	
	UPlayerInputSubsystem* GetPlayerInputSubsystem() const;

};


