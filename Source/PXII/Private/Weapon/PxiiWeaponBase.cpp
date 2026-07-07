// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PxiiWeaponBase.h"

// Sets default values
APxiiWeaponBase::APxiiWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SKWeapon = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("SKWeaponMesh"));
	SKWeapon->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APxiiWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APxiiWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

