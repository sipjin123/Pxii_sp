#pragma once
#include "PxiiInteractableBase.generated.h"

UCLASS(Blueprintable)
class PXII_API APxiiInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InteractionStarted();
	void InteractionStarted_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InteractionReleased();
	void InteractionReleased_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InteractionCompleted();
	void InteractionCompleted_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetInteractable();
	void ResetInteractable_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartResetTimer();
	void StartResetTimer_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ToggleInteractable(bool isEnable);
	void ToggleInteractable_Implementation(bool isEnable);

	UFUNCTION(BlueprintPure)
	bool IsInteractable();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InteractionTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsInteractableActive = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ReactivationTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool AutoResetOnComplete = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTimerHandle ReactivationTimeHandler;
};
