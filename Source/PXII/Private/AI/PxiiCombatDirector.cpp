// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PxiiCombatDirector.h"

#include "Interface/PxiiCombatInterface.h"

// Sets default values
APxiiCombatDirector::APxiiCombatDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APxiiCombatDirector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APxiiCombatDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APxiiCombatDirector::RegisterSquadMember(AActor* Member, EEnemy EnemyType)
{
	if (!IsValid(Member))
	{
		return;
	}

	for (const FPxiiSquadMember& SquadMember:SquadMembers)
	{
		if (SquadMember.Actor==Member)
		{
			return;
		}
	}

	FPxiiSquadMember NewMember;
	NewMember.Actor=Member;
	NewMember.EnemyType=EnemyType;
	NewMember.CombatValue=GetEnemyCombatValue(EnemyType);
	SquadMembers.Add(NewMember);
}

void APxiiCombatDirector::UnregisterSquadMember(AActor* Member)
{
	if (!IsValid(Member))
	{
		return;
	}

	SquadMembers.RemoveAll([Member](const FPxiiSquadMember& SquadMember)
	{
	   return SquadMember.Actor==Member;
	});
}

float APxiiCombatDirector::GetRemainingSquadStrength() const
{
	float Strength=0.f;

	for (const FPxiiSquadMember& SquadMember:SquadMembers)
	{
		if (!IsValid(SquadMember.Actor))
		{
			continue;
		}

		const float MaxHealth=IPxiiCombatInterface::Execute_OnGetCurrentMaxHealth(SquadMember.Actor);
		const float CurrentHealth=IPxiiCombatInterface::Execute_OnGetCurrentHealth(SquadMember.Actor);

		if (MaxHealth<=0.f)
		{
			continue;
		}

		const float HealthRatio=FMath::Clamp(CurrentHealth/MaxHealth,0.f,1.f);
		Strength+=SquadMember.CombatValue*HealthRatio;
	}

	return Strength;
}

float APxiiCombatDirector::GetInitialSquadStrength() const
{
	float Strength=0.f;

	for (const FPxiiSquadMember& SquadMember:SquadMembers)
	{
		if (!IsValid(SquadMember.Actor))
		{
			continue;
		}

		Strength+=SquadMember.CombatValue;
	}

	return Strength;
}

float APxiiCombatDirector::GetEnemyCombatValue(EEnemy EnemyType) const
{
	switch (EnemyType)
	{
	case EEnemy::Brawler:
		return 1.f;
	case EEnemy::Slinger:
		return 1.5f;
	case EEnemy::Mage:
		return 1.75f;
	case EEnemy::Shielder:
	case EEnemy::Warrior:
		return 2.f;
	default:
		return 1.f;
	}
}

