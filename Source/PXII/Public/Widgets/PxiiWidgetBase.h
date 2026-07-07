// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PxiiWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PXII_API UPxiiWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// ~ Begin UUserWidget interface
	virtual void NativeOnInitialized() override;
	// ~ End UUserWidget interface
};
