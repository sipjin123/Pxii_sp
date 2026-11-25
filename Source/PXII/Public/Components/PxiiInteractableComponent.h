#pragma once
#include "Interactable/PxiiInteractableBase.h"
#include "PxiiInteractableComponent.generated.h"

UCLASS(Blueprintable)
class UPxiiInteractableComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetInteractable(APxiiInteractableBase* interactableItem);
	void SetInteractable_Implementation(APxiiInteractableBase* interactableItem);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteractionStarted();
	void OnInteractionStarted_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnInteractionReleased();
	void OnInteractionReleased_Implementation();
	
protected:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APxiiInteractableBase> Interactable;
};
