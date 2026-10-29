// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/PxiiPropsBase.h"

// Sets default values
APxiiPropsBase::APxiiPropsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APxiiPropsBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APxiiPropsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

