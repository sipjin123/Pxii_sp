// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PxiiCharacterBase.h"
#include "GAS/PxiiAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Components/PxiiCombatComponent.h"

DEFINE_LOG_CATEGORY(LogTempBaseCharacter);
// Sets default values
APxiiCharacterBase::APxiiCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent=CreateDefaultSubobject<UPxiiAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void APxiiCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTempBaseCharacter, Warning, TEXT("Char Init 5-Cpp"));
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GrantAllAbilities();
	}
	else
	{
		UE_LOG(LogTempBaseCharacter, Error, TEXT("Missing Ability Component!"));
	}
	if(CombatComponentClass)
	{
		if(CombatComponent=NewObject<UPxiiCombatComponent>(this,CombatComponentClass))
		{
			CombatComponent->RegisterComponent();
		}
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
