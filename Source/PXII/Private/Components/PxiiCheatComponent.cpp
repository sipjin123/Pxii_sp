// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PxiiCheatComponent.h"

DEFINE_LOG_CATEGORY(LogCheatComp);
// Sets default values for this component's properties
UPxiiCheatComponent::UPxiiCheatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPxiiCheatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogCheatComp, Log, TEXT("Cheat Component Init C++"));
}


// Called every frame
void UPxiiCheatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

