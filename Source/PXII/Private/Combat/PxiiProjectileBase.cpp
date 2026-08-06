// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/PxiiProjectileBase.h"

#include "AbilitySystemInterface.h"
#include "Components/PxiiCombatComponent.h"
#include "Components/PxiiPlayerCombatComponent.h"
#include "Components/SphereComponent.h"
#include "Subsystem/ProjectileSubsystem.h"
#include "Utility/PXIILogUtility.h"

// Sets default values
APxiiProjectileBase::APxiiProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &APxiiProjectileBase::OnHit);
	CollisionComponent->InitSphereRadius(8.0f);
	RootComponent = CollisionComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f; // override per-weapon in BP (0 for hitscan-like lasers)
	ProjectileMovement->MaxSpeed = 0.0f;
	ProjectileMovement->UpdatedComponent = CollisionComponent;
}

bool APxiiProjectileBase::GetIsInUse()
{
	return bIsInUse;
}

// Called when the game starts or when spawned
void APxiiProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnTrailEffects();

	GetWorldTimerManager().SetTimer(LifetimeTimerHandle, this, &APxiiProjectileBase::OnLifetimeExpired, Lifetime, false);
}

void APxiiProjectileBase::ApplyDamage_Implementation(AActor* HitActor, const FHitResult& Hit)
{
	if(DamageGE && !InstigatorASC)
	{
		return;
	}

	if(ExplosionRadius > 0.0f)
	{
		TArray<FOverlapResult> Overlaps;
		FCollisionShape Sphere = FCollisionShape::MakeSphere(ExplosionRadius);

		GetWorld()->OverlapMultiByChannel(Overlaps, Hit.ImpactPoint, FQuat::Identity, ECC_Pawn, Sphere);
		for (const FOverlapResult& Overlap : Overlaps)
		{
			if (AActor* OverlapActor = Overlap.GetActor())
			{
				ApplyDamageEffectToActor(OverlapActor, Hit);
			}
		}
	}
	else
	{
		ApplyDamageEffectToActor(HitActor, Hit);
	}
}

void APxiiProjectileBase::SpawnImpactEffects_Implementation(const FHitResult& Hit)
{
	if(ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ImpactEffect,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation(),
			FVector(1.0f),
			true,            
			true,            
			ENCPoolMethod::AutoRelease, 
			true             
		);
	}
	
}

void APxiiProjectileBase::SpawnTrailEffects_Implementation()
{
	if(TrailEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			TrailEffect,
			GetRootComponent(),
			TrailSocket,
			TrailLocationOffset,
			TrailRotationOffset,
			EAttachLocation::KeepRelativeOffset,
			true);
	}
}

void APxiiProjectileBase::InitializeProjectile_Implementation(float BaseDamage, const FVector& Direction, float Speed,
	AActor* InInstigator, AActor* InWeaponOwner)
{
	InstigatorActor = InInstigator;
	WeaponOwner = InWeaponOwner;
	BaseDMG = BaseDamage;

	InstigatorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InWeaponOwner);

	FVector NormalizedDir = Direction.GetSafeNormal();
	SetActorRotation(NormalizedDir.Rotation());
	
	ProjectileMovement->Velocity = NormalizedDir * Speed;
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;

	if (InstigatorActor)
	{
		CollisionComponent->IgnoreActorWhenMoving(InstigatorActor, true);
	}
	if (WeaponOwner && WeaponOwner != InstigatorActor)
	{
		CollisionComponent->IgnoreActorWhenMoving(WeaponOwner, true);
	}
}

void APxiiProjectileBase::ApplyDamageEffectToActor_Implementation(AActor* TargetActor, const FHitResult& result)
{
	if(DamageGE == nullptr)
	{
		UPxiiCombatComponent* SelfCombatComp = IPxiiCombatInterface::Execute_GetCombatComponent(WeaponOwner);
		if (!SelfCombatComp)
		{
			return;   
		}
	
		if (UPxiiPlayerCombatComponent* PlayerCombatComp = Cast<UPxiiPlayerCombatComponent>(SelfCombatComp))
		{
			PlayerCombatComp->ProcessUnitDamage(TargetActor, result.ImpactPoint, 5.f,  EDamageSource::Range);
		}
	}
	else
	{
		FGameplayEffectContextHandle ContextHandle = InstigatorASC->MakeEffectContext();
		ContextHandle.AddSourceObject(this);
		ContextHandle.AddHitResult(result);
		ContextHandle.AddInstigator(InstigatorActor, WeaponOwner);
		
		IAbilitySystemInterface* TargetASI = Cast<IAbilitySystemInterface>(TargetActor);
		if (!TargetASI)
		{
			return;
		}

		UAbilitySystemComponent* TargetASC = TargetASI->GetAbilitySystemComponent();
		if (!TargetASC)
		{
			return;
		}
		
		FGameplayEffectSpecHandle SpecHandle = InstigatorASC->MakeOutgoingSpec(DamageGE->StaticClass(), 1.0f, ContextHandle);
		if(SpecHandle.IsValid())
		{
			InstigatorASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
		}
	}

	/* DZ - Reapply this to combat registry
	const UPxiiAttributeSet* AttributeSet = IPxiiCombatInterface::Execute_GetAttributeSet(CurrHitActor);
	PXII_LOG(ELogCategory::Trace, Warning, TEXT("TRACE HIT Damage: {%f}"), AttributeSet->Health.GetCurrentValue());
	


	FGameplayEffectContextHandle ContextHandle = InstigatorASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	ContextHandle.AddHitResult(result);
	ContextHandle.AddInstigator(InstigatorActor, WeaponOwner);

	if(!overrideEffect)
	{
		FGameplayEffectSpecHandle SpecHandle = InstigatorASC->MakeOutgoingSpec(DamageGE, 1.0f, ContextHandle);

		if (SpecHandle.IsValid())
		{
			// Falloff example: scale damage by distance for AoE explosions
			if (ExplosionRadius > 0.0f)
			{
				float Distance = FVector::Dist(result.ImpactPoint, TargetActor->GetActorLocation());
				if(EnableDamageFalloff)
				{
					float FalloffScalar = FMath::Clamp(1.0f - (Distance / ExplosionRadius), 0.0f, 1.0f);
					SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), FalloffScalar);				
				}
			}

			InstigatorASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
		}
	}
	else
	{
		FGameplayEffectSpecHandle SpecHandle = InstigatorASC->MakeOutgoingSpec(OverrideDamageClass->StaticClass(), 1.0f, ContextHandle);
		if(SpecHandle.IsValid())
		{
			InstigatorASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
		}
	}
	//*/
}

void APxiiProjectileBase::SetIsInUse(bool InIsInUse)
{
	bIsInUse = InIsInUse;

	this->SetActorEnableCollision(bIsInUse);
	this->SetActorHiddenInGame(!bIsInUse);
	this->SetActorTickEnabled(bIsInUse);

	// Always make sure timer is clear, no matter true or false
	if (LifetimeTimerHandle.IsValid() && GetWorldTimerManager().IsTimerActive(LifetimeTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(LifetimeTimerHandle);
	}

	// Only start timer when needed
	if (bIsInUse)
	{
		GetWorldTimerManager().SetTimer(
			LifetimeTimerHandle,
			[this]() {
				this->SetIsInUse(false);

				UProjectileSubsystem* ProjectileSubsystem = this->GetWorld()->GetSubsystem<UProjectileSubsystem>();
				
				if (ProjectileSubsystem)
				{
					if(ProjectileSubsystem->bPrintDebugLog)
					{
						PXII_LOG(ELogCategory::Combat, Log, TEXT("Projectile returned to pool"));
					}

					// Debug
					ProjectileSubsystem->OnProjectileReturnPool.Broadcast();
					// Debug
				}
			},
			Lifetime,
			false
		);
	}
}

void APxiiProjectileBase::OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	PXII_LOG(ELogCategory::Projectile, Log, TEXT("Bullet HIT"));
	
	if (OtherActor == this || OtherActor == InstigatorActor)
	{
		return;
	}

	ApplyDamage(OtherActor, Hit);
	SpawnImpactEffects(Hit);
	Destroy();
}

void APxiiProjectileBase::OnLifetimeExpired()
{
	Destroy();
}

