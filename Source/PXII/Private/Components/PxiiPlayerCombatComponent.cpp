// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PxiiPlayerCombatComponent.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/PxiiTags.h"
#include "Enum/PxiiDamageType.h"

UPxiiPlayerCombatComponent::UPxiiPlayerCombatComponent()
{
}

// Called when the game starts
void UPxiiPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("CombatComponentStarted"));
	// ...
}

// Called every frame
void UPxiiPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
