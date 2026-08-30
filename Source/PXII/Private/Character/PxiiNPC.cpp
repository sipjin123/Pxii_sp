// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PxiiNPC.h"

#include "Components/PxiiCombatComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Enum/PxiiDamageType.h"
#include "GAS/PxiiAbilitySystemComponent.h"
#include "GAS/PxiiAttributeSet.h"
#include "Subsystem/PxiiCombatRegistrySubsystem.h"

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

FVector APxiiNPC::GetAimSocketLocation_Implementation() const
{
	if(USkeletalMeshComponent* SkeletalMesh = GetMesh())
	{
		if(SkeletalMesh->DoesSocketExist(AimSocketName))
		{
			return SkeletalMesh->GetSocketLocation(AimSocketName);
		}
	}

	FBoxSphereBounds Bounds = GetMesh()->GetBounds();
	float Height = Bounds.BoxExtent.Z;
	return GetActorLocation() + Height;
}

TArray<FVector> APxiiNPC::GetWeakpointLocations_Implementation() const
{
	TArray<FVector> Locations;

	if (USkeletalMeshComponent* SkeletalMesh = GetMesh())
	{
		for (const FName& SocketName : WeakPointSocketNames)
		{
			if (SkeletalMesh->DoesSocketExist(SocketName))
			{
				Locations.Add(SkeletalMesh->GetSocketLocation(SocketName));
			}
		}
	}

	return Locations;
}

bool APxiiNPC::IsLockable_Implementation() const
{
	return bIsCurrentlyLockable && !IsPendingKillPending();
}

float APxiiNPC::GetThreatPriority_Implementation() const
{
	return ThreatPriority;
}

AActor* APxiiNPC::GetCombatTarget_Implementation() const
{
	return LastKnownTarget;
}

// Called when the game starts or when spawned
void APxiiNPC::BeginPlay()
{
	Super::BeginPlay();
	CombatComponent = FindComponentByClass<UPxiiCombatComponent>();
	if(CombatComponent)
	{
		//UE_LOG(LogTempBaseCharacter, Warning, TEXT("Combat Component Registered"));
		//CombatComponent->RegisterComponent();
		AttributeSet = AbilitySystemComponent->GetSet<UPxiiAttributeSet>();
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->GrantAllAbilities();
			AbilitySystemComponent->GrantAllPlayerEffects();
			
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPxiiAttributeSet::GetStaggerMeterAttribute()).AddUObject(this, &ThisClass::OnStaggerMeterChanged);

			UPxiiCombatRegistrySubsystem* combatSubsystem = GetWorld()->GetSubsystem<UPxiiCombatRegistrySubsystem>();
			combatSubsystem->GiveGlobalAbilitySet(AbilitySystemComponent);
		}
	}
	else
	{
		UE_LOG(LogTempBaseCharacter, Error, TEXT("Missing Ability Component!"));
	}
}

void APxiiNPC::OnStaggerMeterChanged(const FOnAttributeChangeData& Data)
{
	//UE_LOG(LogTemp, Warning, TEXT("StaggerMeter: %f -> %f"), Data.OldValue, Data.NewValue);
	if (Data.NewValue >= 100.f)
	{
		OnHitEffectType.Broadcast(EHitEffectType::Stagger, 0);
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

	if (IsAttacking)
	{
		FVector SocketLocation = GetMesh()->GetSocketLocation(CombatComponent->SocketTarget);
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, SocketLocation]()
		{
			//Multicast_SpawnSimulatedHit(SocketLocation);

			// Sends over a broadcast to process logic on the blueprint
			TraceTarget.Broadcast(SocketLocation);
		});
	}
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

void APxiiNPC::EnableAttackState_Implementation()
{
	IsAttacking = true;
	OnAttackState.Broadcast(true, 0);
}

void APxiiNPC::DisableAttackState_Implementation()
{
	IsAttacking = false;
	OnAttackState.Broadcast(false, 0);
}

EFactionType APxiiNPC::GetFaction_Implementation()
{
	return FactionType;
}

void APxiiNPC::RegisterLastKnownAttacker_Implementation(AActor* Target, float Damage)
{
	IPxiiCombatInterface::RegisterLastKnownAttacker_Implementation(Target, Damage);
	LastKnownAttacker = Target;
}

float APxiiNPC::OnGetCurrentHealth_Implementation()
{
	return AttributeSet->Health.GetCurrentValue();
}

float APxiiNPC::OnGetCurrentMaxHealth_Implementation()
{
	
	return AttributeSet->MaxHealth.GetCurrentValue();
}
