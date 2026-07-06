// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PxiiCharacter.h"

#include "AbilitySystemComponent.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"

DEFINE_LOG_CATEGORY(LogTempCharacter);
// Called when the game starts or when spawned
void APxiiCharacter::BeginPlay()
{
	UE_LOG(LogTempCharacter, Warning, TEXT("Char Init 4-Cpp"));
	Super::BeginPlay();

	if (GetAbilitySystemComponent())
	{
		UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: ASC INITIALIZED"));
		GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	}
}

void APxiiCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: Possessed By"));
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
}

void APxiiCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if (IsAttacking)
	{
		FVector SocketLocation = GetMesh()->GetSocketLocation(TEXT("palm_r_Socket"));
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, SocketLocation]()
		{
			//Multicast_SpawnSimulatedHit(SocketLocation);

			// Sends over a broadcast to process logic on the blueprint
			TraceTarget.Broadcast(SocketLocation);
		});
	}
}

APxiiWeaponBase* APxiiCharacter::GetWeaponBase()
{
	return nullptr;
}
