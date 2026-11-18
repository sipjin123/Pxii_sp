// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PxiiCharSimulatorComponent.generated.h"


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Exploration,
	Range,
	Melee,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterStateUpdated, ECharacterState, NewState);


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class PXII_API UPxiiCharSimulatorComponent : public UActorComponent
{
	GENERATED_BODY()



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UPxiiCharSimulatorComponent();

	UPROPERTY(BlueprintAssignable)
	FOnCharacterStateUpdated OnCharacterStateUpdated;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	
};
