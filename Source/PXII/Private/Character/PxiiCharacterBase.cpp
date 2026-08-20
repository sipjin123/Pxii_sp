// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PxiiCharacterBase.h"
#include "GAS/PxiiAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "MoverComponent.h"
#include "Components/PxiiAimAssistComponent.h"
#include "Components/PxiiCombatComponent.h"
#include "GAS/PxiiAttributeSet.h"
#include "Subsystem/PxiiCombatRegistrySubsystem.h"

DEFINE_LOG_CATEGORY(LogTempBaseCharacter);
// Sets default values
APxiiCharacterBase::APxiiCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent=CreateDefaultSubobject<UPxiiAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UPxiiAttributeSet>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void APxiiCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTempBaseCharacter, Warning, TEXT("Char Init 5-Cpp"));
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay AttributeSet = %s"), *GetNameSafe(AttributeSet));
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GrantAllAbilities();
		AbilitySystemComponent->GrantAllPlayerEffects();

		UPxiiCombatRegistrySubsystem* combatSubsystem = GetWorld()->GetSubsystem<UPxiiCombatRegistrySubsystem>();
		combatSubsystem->GiveGlobalAbilitySet(AbilitySystemComponent);
	}
	else
	{
		UE_LOG(LogTempBaseCharacter, Error, TEXT("Missing Ability Component!"));
	}

	CombatComponent = FindComponentByClass<UPxiiCombatComponent>();
	if(CombatComponent)
	{
		//UE_LOG(LogTempBaseCharacter, Warning, TEXT("Combat Component Registered"));
		//CombatComponent->RegisterComponent();
	}
	else
	{
		UE_LOG(LogTempBaseCharacter, Error, TEXT("Missing Ability Component!"));
	}

	
	MeshRef = FindComponentByClass<USkeletalMeshComponent>();
	if(MeshRef)
	{
		UE_LOG(LogTempBaseCharacter, Warning, TEXT("USkeletalMesh Component Registered"));
	}
	else
	{
		UE_LOG(LogTempBaseCharacter, Error, TEXT("Missing USkeletalMesh Component!"));
	}

	MoverComponent = FindComponentByClass<UMoverComponent>();
	if (MoverComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("MoverComponent found"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Missing MoverComponent!"));
	}
}

// Called every frame
void APxiiCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APxiiCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* APxiiCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPxiiCombatComponent* APxiiCharacterBase::GetCombatComponent_Implementation() const
{
	return CombatComponent;
}

const UPxiiAttributeSet* APxiiCharacterBase::GetAttributeSet_Implementation() const
{
	return AttributeSet;
}

UPxiiAimAssistComponent* APxiiCharacterBase::GetAimAssistComponent_Implementation() const
{
	return FindComponentByClass<UPxiiAimAssistComponent>();
}

EFactionType APxiiCharacterBase::GetFaction_Implementation()
{
	return FactionType;
}

void APxiiCharacterBase::EnableAttackState_Implementation()
{
	IsAttacking = true;
}

void APxiiCharacterBase::DisableAttackState_Implementation()
{
	IsAttacking = false;
}

void APxiiCharacterBase::ReleaseAggro_Implementation()
{
	CurrentAggroCount = FMath::Max(0, CurrentAggroCount - 1);
	if (LogAggro)
		UE_LOG(LogTemp, Log, TEXT("Aggro Released"));
}

void APxiiCharacterBase::TryAcquireAggro_Implementation(bool& bOutSuccess)
{
	if (CanGetAggro_Implementation())
	{
		CurrentAggroCount++;
		if (LogAggro)
			UE_LOG(LogTemp, Log, TEXT("Aggro acquired. Current count: %d"), CurrentAggroCount);
		bOutSuccess = true;
	}
	else
	{
		bOutSuccess = false;
		if (LogAggro)
			UE_LOG(LogTemp, Log, TEXT("Aggro Failed to get. Current count: %d"), CurrentAggroCount);
	}
}

bool APxiiCharacterBase::CanGetAggro_Implementation()
{
	return CurrentAggroCount < MaxAggroSlots;
}

int32 APxiiCharacterBase::GetAvailableAggroSlots()
{
	return MaxAggroSlots - CurrentAggroCount;
}

