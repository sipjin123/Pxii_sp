// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PxiiCharSimulatorComponent.h"

#include "TimerManager.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UPxiiCharSimulatorComponent::UPxiiCharSimulatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPxiiCharSimulatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPxiiCharSimulatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPxiiCharSimulatorComponent::SetCharacterState_Implementation(ECharacterState state)
{
	CurrentState = state;
	UWorld* World = GetWorld();

	if(!World)
	{
		return;
	}

	World->GetTimerManager().ClearTimer(StateHandler);

	if(CurrentState == ECharacterState::Exploration)
	{
		return;	
	}

	World->GetTimerManager().SetTimer(StateHandler,	this, 	&UPxiiCharSimulatorComponent::ResetToExploration_Implementation,
	ResetTimer,false);
}

void UPxiiCharSimulatorComponent::ResetToExploration_Implementation()
{
	CurrentState = ECharacterState::Exploration;
}

ECharacterState UPxiiCharSimulatorComponent::GetCharacterState()
{
	return CurrentState;
}

