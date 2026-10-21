// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/PxiiPillarActor.h"

// Sets default values
APxiiPillarActor::APxiiPillarActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APxiiPillarActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APxiiPillarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APxiiPillarActor::SetHitCount(int32 NewValue)
{
	if (HitCount==NewValue)
	{
		return;
	}
	HitCount=NewValue;
	OnHitCountChanged(HitCount);
	ObjHealthChanged.Broadcast(HitCount);
}

void APxiiPillarActor::SetMaxHitCount(int32 NewValue)
{
	if (MaxHitCount==NewValue)
	{
		return;
	}
	MaxHitCount=NewValue;
	OnMaxHitCountChanged(MaxHitCount);
}