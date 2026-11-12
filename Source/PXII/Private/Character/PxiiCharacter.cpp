// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PxiiCharacter.h"

#include "AbilitySystemComponent.h"
#include "TimerManager.h"
#include "Components/PxiiCombatComponent.h"
#include "Components/PxiiPlayerCombatComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DefaultMovementSet/InstantMovementEffects/BasicInstantMovementEffects.h"
#include "GAS/PxiiAbilitySystemComponent.h"
#include "Utility/PXIILogUtility.h"

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
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

	//*DZ_NOTE: Remove this when item equip is implemented must not be granted by default.
	GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Pxii.Weapon.Type.Melee")));
	//*/
}


void APxiiCharacter::OnJumpStarted_Implementation()
{
	
}

void APxiiCharacter::OnJumpTriggered_Implementation()
{
	
}

void APxiiCharacter::SnapTurn_Implementation(float AngleRotation)
{
	if (!MoverComponent)
		return;

	// Get current Mover transform
	const FTransform CurrentTransform =
		MoverComponent->GetUpdatedComponentTransform();

	// Calculate new rotation
	FRotator NewRotation = CurrentTransform.Rotator();

	NewRotation.Yaw = FMath::UnwindDegrees(AngleRotation);

	// Create instant movement effect
	TSharedPtr<FTeleportEffect> TurnEffect =
		MakeShared<FTeleportEffect>();

	// Preserve current location
	TurnEffect->TargetLocation = CurrentTransform.GetLocation();

	// Apply new rotation
	TurnEffect->TargetRotation = NewRotation;
	TurnEffect->bUseActorRotation = false;

	// Queue effect through Mover
	MoverComponent->QueueInstantMovementEffect(TurnEffect);
}

void APxiiCharacter::ProcessBlasterFired_Implementation()
{

}

USkeletalMeshComponent* APxiiCharacter::GetMesh()
{
	return MeshRef;
}

void APxiiCharacter::SetIsADSEnabled_Implementation(bool isEnabled)
{
	bIsADSActive = isEnabled; 
}

APxiiWeaponRange* APxiiCharacter::GetWeaponBaseRange_Implementation()
{
	return CurrentWeaponRanged;
}

APxiiWeaponMelee* APxiiCharacter::GetWeaponBaseMelee_Implementation()
{
	return CurrentWeaponMelee;
}

void APxiiCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

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

void APxiiCharacter::ProcessDamageData_Implementation(AActor* SourceActor, float Damage, float DamageSource)
{
	EDamageSource DamageSourceType = static_cast<EDamageSource>(FMath::RoundToInt(DamageSource));
	Super::ProcessDamageData_Implementation(SourceActor, Damage, DamageSource);
}

float APxiiCharacter::GetRangeWeaponDamage(float hitDistance, float bulletBonusDamage, bool isCritical)
{
	if(!CurrentWeaponRanged)
	{
		return 0.0f;
	}

	float playerDMG = AbilitySystemComponent->GetNumericAttribute(UPxiiAttributeSet::GetAttackDamageAttribute());
	float dmgMultiplier = AbilitySystemComponent->GetNumericAttribute(UPxiiAttributeSet::GetAttackDamageMultiplierAttribute());
	float blasterMultiplier = AbilitySystemComponent->GetNumericAttribute(UPxiiAttributeSet::GetBlasterMultiplierAttribute());

	return (playerDMG * dmgMultiplier) + CurrentWeaponRanged->GetDamage(hitDistance, bulletBonusDamage, blasterMultiplier, isCritical);
}

float APxiiCharacter::GetArmbladeDamage(float bonusDamage)
{
	if(!CurrentWeaponMelee)
	{
		return 0.0f;
	}

	float playerDMG = AbilitySystemComponent->GetNumericAttribute(UPxiiAttributeSet::GetAttackDamageAttribute());
	float dmgMultiplier = AbilitySystemComponent->GetNumericAttribute(UPxiiAttributeSet::GetAttackDamageMultiplierAttribute());
	float armbladeMultiplier = AbilitySystemComponent->GetNumericAttribute(UPxiiAttributeSet::GetArmbladeMultiplierAttribute());

	PXII_LOG(ELogCategory::Combat, Log, TEXT("DZ_LOG:: playerDMG : %f, dmgMult: %f armbladeMult: %f"), playerDMG, dmgMultiplier, armbladeMultiplier);
	return (playerDMG * dmgMultiplier) + CurrentWeaponMelee->GetArmbladeDamage(bonusDamage, armbladeMultiplier);
}

