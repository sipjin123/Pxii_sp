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
		UE_LOG(LogTempCharacter, Log, TEXT("DZ_LOG:: ASC INITIALIZED"));
		GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	}
}

void APxiiCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTempCharacter, Log, TEXT("DZ_LOG:: Possessed By"));
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
}

void APxiiCharacter::InitAttributeListener()
{
	if (AbilitySystemComponent)
	{
		for(FGameplayAttribute attrib : Attributes)
		{
			GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(attrib).AddLambda(
			[this, attrib](const FOnAttributeChangeData& Data)
			{
				OnAttributeChanged(attrib, Data);
			});
		}

	}
}

void APxiiCharacter::OnAttributeChanged(FGameplayAttribute Attribute, const FOnAttributeChangeData& Data)
{
	if(Attribute ==  UPxiiAttributeSet::GetMaxHealthAttribute())
	{
		OnMaxHealthUpdated.Broadcast(Data.NewValue);
	}
	else if(Attribute == UPxiiAttributeSet::GetMaxAmmoAttribute())
	{
		OnMaxAmmoUpdated.Broadcast(Data.NewValue);
	}
	else if(Attribute == UPxiiAttributeSet::GetMaxManaAttribute())
	{
		OnMaxManaUpdated.Broadcast(Data.NewValue);
	}
	else if(Attribute == UPxiiAttributeSet::GetMovementSpeedAttribute())
	{
		OnMovementSpeedUpdated.Broadcast(Data.NewValue);
	}
}

USkeletalMeshComponent* APxiiCharacter::GetMesh()
{
	return MeshRef;
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

APxiiWeaponRange* APxiiCharacter::GetWeaponRanged()
{
	return CurrentWeaponRanged;
}

APxiiWeaponMelee* APxiiCharacter::GetWeaponBaseMelee()
{
	return CurrentWeaponMelee;
}

