// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PxiiNPC.h"

// Sets default values
APxiiNPC::APxiiNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APxiiNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APxiiNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APxiiNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

