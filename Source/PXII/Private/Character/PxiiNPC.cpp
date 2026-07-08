// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PxiiNPC.h"

#include "Components/PxiiCombatComponent.h"
#include "GAS/PxiiAbilitySystemComponent.h"
#include "GAS/PxiiAttributeSet.h"

// Sets default values
APxiiNPC::APxiiNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent=CreateDefaultSubobject<UPxiiAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UPxiiAttributeSet>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void APxiiNPC::BeginPlay()
{
	Super::BeginPlay();
	CombatComponent = FindComponentByClass<UPxiiCombatComponent>();
	if(CombatComponent)
	{
		UE_LOG(LogTempBaseCharacter, Warning, TEXT("Combat Component Registered"));
		//CombatComponent->RegisterComponent();
		AttributeSet = AbilitySystemComponent->GetSet<UPxiiAttributeSet>();
	}
	else
	{
		UE_LOG(LogTempBaseCharacter, Error, TEXT("Missing Ability Component!"));
	}
}

// Called to bind functionality to input
void APxiiNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called every frame
void APxiiNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* APxiiNPC::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPxiiCombatComponent* APxiiNPC::GetCombatComponent_Implementation() const
{
	return CombatComponent;
}

const UPxiiAttributeSet* APxiiNPC::GetAttributeSet_Implementation() const
{
	return AttributeSet;
}
