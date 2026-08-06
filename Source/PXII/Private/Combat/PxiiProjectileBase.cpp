// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/PxiiProjectileBase.h"

#include "AbilitySystemInterface.h"
#include "TimerManager.h"
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
}

void APxiiProjectileBase::ApplyDamage_Implementation(AActor* HitActor, const FHitResult& Hit)
{
	PXII_LOG(ELogCategory::Projectile, Log, TEXT("Applying Damage"));
	
	if(DamageGE && !InstigatorASC)
	{
		return;
	}

	if(ExplosionRadius > 0.0f)
	{
		PXII_LOG(ELogCategory::Projectile, Log, TEXT("AOE DETECTED getting actor within impact radius"));

		FCollisionQueryParams collisionParam;
		collisionParam.AddIgnoredActor(WeaponOwner);
		
		TArray<FOverlapResult> Overlaps;
		FCollisionShape Sphere = FCollisionShape::MakeSphere(ExplosionRadius);

		GetWorld()->OverlapMultiByChannel(Overlaps, Hit.ImpactPoint, FQuat::Identity, ECC_Pawn, Sphere, collisionParam);
		for (const FOverlapResult& Overlap : Overlaps)
		{
			if (AActor* OverlapActor = Overlap.GetActor())
			{
				if(OverlapActor == WeaponOwner)
				{
					PXII_LOG(ELogCategory::Projectile, Log, TEXT("Skipping owner in radius damage"));
					continue;
				}
				
				PXII_LOG(ELogCategory::Projectile, Log, TEXT("Applying Damage to Target: %s"), *GetNameSafe(OverlapActor));
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
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect,	Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation(), FVector(1.0f),	true, true,  
			ENCPoolMethod::AutoRelease, true             
		);
	}
}

void APxiiProjectileBase::SpawnTrailEffects_Implementation()
{
	if(TrailEffect && !TrailComp)
	{
		TrailComp = UNiagaraFunctionLibrary::SpawnSystemAttached(TrailEffect, GetRootComponent(), TrailSocket,
			TrailLocationOffset, TrailRotationOffset, EAttachLocation::KeepRelativeOffset,true);

		FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		TrailComp->AttachToComponent(RootComponent, rules);
		TrailComp->SetRelativeLocation(FVector::ZeroVector);
		
		TrailComp->Deactivate();
		TrailComp->ResetSystem();
	}
}

void APxiiProjectileBase::InitializeProjectile_Implementation(float BaseDamage, const FVector& Direction, float Speed,
	AActor* InInstigator, AActor* InWeaponOwner)
{
	PXII_LOG(ELogCategory::Projectile, Log, TEXT("INITALIZED: %s"), *GetName());
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
			float dist = FVector::Dist(result.ImpactPoint, TargetActor->GetActorLocation());
			float fallOffModifier = 1.0f;
			if (ExplosionRadius > 0.0f)
			{
				DrawDebugSphere(GetWorld(), result.ImpactPoint, ExplosionRadius, 32, FColor::Red,
					false, 2.0f, 0, 1.0f);
				if(EnableDamageFalloff)
				{
					fallOffModifier = FMath::Clamp(1.0f - (dist / ExplosionRadius), 0.0f, 1.0f);
				}
			}
			
			const float finalDMG = BaseDMG * fallOffModifier;
			
			PXII_LOG(ELogCategory::Projectile, Log, TEXT("Apply Damage to %s -> Final DMG: %f"), *GetNameSafe(TargetActor), finalDMG);
			PlayerCombatComp->ProcessUnitDamage(TargetActor, result.ImpactPoint, finalDMG,  EDamageSource::Range);
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

void APxiiProjectileBase::SetProjectileTag(FGameplayTag inTag)
{
	PoolTag = inTag;
	SpawnTrailEffects();
}

FGameplayTag APxiiProjectileBase::GetPoolTag()
{
	return PoolTag;
}

void APxiiProjectileBase::SetIsInUse(bool InIsInUse)
{
	PXII_LOG(ELogCategory::Projectile, Log, TEXT("SET TO USE[%s] : %s "), *GetName(), InIsInUse ? TEXT("TRUE") : TEXT("FALSE"));

	bIsInUse = InIsInUse;

	SetActorEnableCollision(bIsInUse);
	SetActorHiddenInGame(!bIsInUse);
	SetActorTickEnabled(bIsInUse);
	
	if(!bIsInUse)
	{
		CollisionComponent->ClearMoveIgnoreActors();
		ProjectileMovement->StopMovementImmediately();
	}
	else
	{
		ProjectileMovement->SetUpdatedComponent(CollisionComponent);
		ProjectileMovement->SetComponentTickEnabled(true);
		ProjectileMovement->SetActive(true);
		TrailComp->ReinitializeSystem();
		TrailComp->Activate(true);
	}

	// Always make sure timer is clear, no matter true or false
	if (LifetimeTimerHandle.IsValid() && GetWorldTimerManager().IsTimerActive(LifetimeTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(LifetimeTimerHandle);
	}

	// Only start timer when needed
	if (bIsInUse)
	{
		GetWorldTimerManager().ClearTimer(LifetimeTimerHandle);
		GetWorldTimerManager().SetTimer(
			LifetimeTimerHandle,[this]
			{
				ReturnProjecileToPool();	
			},
			Lifetime,
			false
		);
	}
}

void APxiiProjectileBase::OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	PXII_LOG(ELogCategory::Projectile, Log, TEXT("Bullet HIT: %s"), *GetName());
	
	if (OtherActor == this || OtherActor == InstigatorActor)
	{
		return;
	}

	ApplyDamage(OtherActor, Hit);
	SpawnImpactEffects(Hit);
	ReturnProjecileToPool();
}

void APxiiProjectileBase::OnLifetimeExpired()
{
	// ReturnProjecileToPool(); 
}

void APxiiProjectileBase::ReturnProjecileToPool_Implementation()
{
	GetWorldTimerManager().ClearTimer(LifetimeTimerHandle);

	if(TrailComp)
	{
		TrailComp->Deactivate();
		TrailComp->ResetSystem();		
	}

	SetIsInUse(false);

	OnReturnToPool.Broadcast(this);
}

