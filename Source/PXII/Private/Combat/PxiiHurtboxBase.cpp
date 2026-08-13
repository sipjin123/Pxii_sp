// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/PxiiHurtboxBase.h"

// Sets default values
APxiiHurtboxBase::APxiiHurtboxBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APxiiHurtboxBase::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetParentActor();
	UE_LOG(LogTemp, Log, TEXT("Hurtbox Owner is: %s"), *Owner.GetName());
}

// Called every frame
void APxiiHurtboxBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APxiiHurtboxBase::ApplyDamage_Implementation(AActor* Source, float Magnitude, int32 Payload)
{
	IPxiiDamageableInterface::ApplyDamage_Implementation(Source, Magnitude, Payload);
	OnPartHit.Broadcast(Source,Magnitude,Payload);
	UE_LOG(LogTemp, Log, TEXT("Apply Damage Interface: %f - %d"), Magnitude, Payload);
}

EDamagePart APxiiHurtboxBase::GetPartType_Implementation()
{
	return DamagePart;
}

