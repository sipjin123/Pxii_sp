// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PxiiCheatComponent.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(LogCheatComp, Log, All);
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class PXII_API UPxiiCheatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPxiiCheatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
