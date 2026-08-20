#pragma once
#include "GameFramework/Actor.h"
#include "PxiiCustomizationHandler.generated.h"

UCLASS(Blueprintable)
class PXII_API APxiiCustomizationHandler : public AActor
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OpenCustomization();
	
	void OpenCustomization_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CloseCustomization();
	
	void CloseCustomization_Implementation();

	UFUNCTION(BlueprintPure)
	bool GetCustomizationActive();
	
private:

	UPROPERTY()
	bool CustomizationActive = false;
};
