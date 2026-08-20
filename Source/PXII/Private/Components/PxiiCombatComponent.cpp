// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PxiiCombatComponent.h"
#include "AbilitySystemComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/PxiiTags.h"
#include "Enum/PxiiDamageType.h"
#include "Interface/PxiiDamageableInterface.h"
#include "Subsystem/PxiiCombatRegistrySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/PxiiDebugTraceBPLibrary.h"

// Sets default values for this component's properties
UPxiiCombatComponent::UPxiiCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPxiiCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//UE_LOG(LogTemp, Warning, TEXT("CombatComponent Init Cpp"));
	
	CharacterRef = Cast<APawn>(GetOwner());
	ActorsToIgnore.Add(GetOwner());
	//DrawDebugTrace = EDrawDebugTrace::ForDuration;
	//UseDepthSlash = true;
	check(CharacterRef);
	AbilitySystemComponent = CharacterRef->FindComponentByClass<UAbilitySystemComponent>();
	// DEV: Code Override, remove later
	//LogSlashLogic = false;
}


// Called every frame
void UPxiiCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPxiiCombatComponent::TriggerProjectileTraceArc(FVector ImpactNormal, FVector ImpactLocation, float ArcHeight)
{

	
}

void UPxiiCombatComponent::TriggerProjectileTrace(FVector ImpactNormal, FVector ImpactLocation, bool drawTrace)
{
	if(drawTrace)
	{
		UPxiiDebugTraceBPLibrary::DrawDebugSphereSimple(this, ImpactLocation, 50.f, FLinearColor::Blue, TraceDuration);
	}
}


void UPxiiCombatComponent::InitializeHitTrace(FVector SocketLoc, float LengthOverride, EHitEffectType NewHitEffectType)
{
	HitEffectType = NewHitEffectType;
	HitTracedActors.Empty();
	TraceIndex = 0;
	HasValidHit = false;
	HasHitReaction = false;
	LengthExtension = LengthOverride;
	if (CharacterRef && CharacterRef->Implements<UPxiiCombatInterface>())
	{
		IPxiiCombatInterface::Execute_EnableAttackState(CharacterRef);
	}
	//CharacterRef->SetIsWeaponMeleeEquipped(true);
}

void UPxiiCombatComponent::ProcessHitTrace(FVector SocketLoc)
{
	// Only process trace logic when a start and end point is generated
	if (TraceIndex > 0)
	{
		ProcessHitTraceLogic(PreviousSocketLocation, SocketLoc);
	}
	TraceIndex++;
	PreviousSocketLocation = SocketLoc;
}

void UPxiiCombatComponent::ProcessHitTraceLogic(FVector StartLoc, FVector EndLoc, bool drawTrace )
{
	if (LogSlashLogic)
		UE_LOG(LogTemp, Warning, TEXT("Slash Trace: TraceLogic Index:%d"), TraceIndex);
	constexpr float Radius = 15.f;

	// Query Setup
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	// Result
	TArray<FHitResult> OutHits;

	if (UseSphere)
	{
		UKismetSystemLibrary::SphereTraceMultiForObjects(GetOwner(),
			StartLoc, EndLoc, Radius, ObjectTypes, false,
			ActorsToIgnore, DrawDebugTrace, OutHits, true,
			FColor::Green, FColor::Red, TraceDuration);
	}
	else
	{
		FVector BoxHalfExtent = BoxExtent + FVector(0.f,0.f,LengthExtension);//FVector(50.f, 5.f, 2.f); // long, thin, flat — sword-shaped
		//FVector BoxHalfExtent = FVector(Radius, Radius, Radius);
		FRotator BoxOrientation = GetOwner()->GetActorRotation();
		if (GetOwner()->Implements<UPxiiCombatInterface>())
		{
			APxiiWeaponMelee* MeleeRef = IPxiiCombatInterface::Execute_GetWeaponBaseMelee(GetOwner());
			if (MeleeRef)
			{
				BoxOrientation = MeleeRef->GetActorRotation();
			}
		}
		

		// NOW compute the tip, using whichever orientation actually ended up correct
		FVector TipLocation = StartLoc + (BoxOrientation.RotateVector(FVector::UpVector) * BoxHalfExtent.Z);
		if(drawTrace)
		{
			DrawDebugSphere(GetWorld(), TipLocation, 10.f, 12, FColor::Yellow, false, TraceDuration);
		}
		
		UKismetSystemLibrary::BoxTraceMultiForObjects(
			GetOwner(),
			StartLoc, EndLoc,
			BoxHalfExtent,        // FVector, replaces Radius
			BoxOrientation,       // FRotator, new param — box needs an orientation
			ObjectTypes, false,
			ActorsToIgnore, DrawDebugTrace, OutHits, true,
			FColor::Green, FColor::Red, TraceDuration
		);
	}
	SlashDataArray = OutHits;
	
	// Handles depth slash logic and adds to SlashDataArray
	if (UseDepthSlash)
	{
		ProcessDepthSlash(EndLoc);
	}
	
	FinalizeHitTraceLogic();
}

void UPxiiCombatComponent::EndHitTrace()
{
	LengthExtension = 0.f;
	if (CharacterRef && CharacterRef->Implements<UPxiiCombatInterface>())
	{
		IPxiiCombatInterface::Execute_DisableAttackState(CharacterRef);
	}
}

void UPxiiCombatComponent::ProcessDepthSlash(FVector EndLoc)
{
	// Query Setup
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	constexpr float DepthRadius = 45.f;
	TArray<FHitResult> DepthOutHits;
	
	USkeletalMeshComponent* Mesh = CharacterRef->FindComponentByClass<USkeletalMeshComponent>();
	if (Mesh)
	{
		FVector HandLocation = Mesh->GetSocketLocation(SocketTarget);
		if (UseSphere)
		{
			UKismetSystemLibrary::SphereTraceMultiForObjects(GetOwner(),
				HandLocation, EndLoc, DepthRadius, ObjectTypes,
				false, ActorsToIgnore, DrawDebugTrace,
				DepthOutHits, true,
				FColor::Blue,FColor::Red, TraceDuration
			);
		}
		else
		{
			FVector BoxHalfExtent = BoxExtent + FVector(0.f,0.f,LengthExtension);;//FVector(50.f, 5.f, 2.f); // long, thin, flat — sword-shaped
			//FVector BoxHalfExtent = FVector(Radius, Radius, Radius);
			FRotator BoxOrientation = GetOwner()->GetActorRotation();
			if (GetOwner()->Implements<UPxiiCombatInterface>())
			{
				APxiiWeaponMelee* MeleeRef = IPxiiCombatInterface::Execute_GetWeaponBaseMelee(GetOwner());
				if (MeleeRef)
				{
					BoxOrientation = MeleeRef->GetActorRotation();
				}
			}
		
			UKismetSystemLibrary::BoxTraceMultiForObjects(
				GetOwner(),
				HandLocation, EndLoc,
				BoxHalfExtent,        // FVector, replaces Radius
				BoxOrientation,       // FRotator, new param — box needs an orientation
				ObjectTypes, false,
				ActorsToIgnore, DrawDebugTrace, DepthOutHits, true,
				FColor::Cyan, FColor::Emerald, TraceDuration
			);
		}
	}
	for (FHitResult OutHitParam : DepthOutHits)
	{
		SlashDataArray.Add(OutHitParam);
	}
}

void UPxiiCombatComponent::FinalizeHitTraceLogic()
{
	for (FHitResult OutHitParam : SlashDataArray)
	{
		const FVector ImpactPoint = OutHitParam.ImpactPoint;
		AActor* CurrHitActor = OutHitParam.GetActor();
		if (IsValid(CurrHitActor))
		{
			if (LogHits)
			{
				UE_LOG(LogTemp, Log, TEXT("Hit Actor: %s"), *CurrHitActor->GetName());
			}
		}
		else
		{
			continue;
		}

		/*
		if (Cast<APxiiCharacter>(OutHitParam.GetActor()))
		{
			DrawDebugSphere(GetWorld(), OutHitParam.ImpactPoint, 100.f, // Radius
				12, FColor::Emerald, false, // Segments (visual quality) // Color // bPersistentLines
				10.f,0, 1.f // LifeTime // DepthPriority // Thickness
			);
		}*/
		
		const bool ImplementsCombatInterface = CurrHitActor->GetClass()->ImplementsInterface(UPxiiCombatInterface::StaticClass());
		const bool ImplementsDamageableInterface = CurrHitActor->GetClass()->ImplementsInterface(UPxiiDamageableInterface::StaticClass());
		if (!ImplementsCombatInterface && !ImplementsDamageableInterface)
		{
			//UE_LOG(LogTemp, Log, TEXT("No Interface for Combat! %s"), *CurrHitActor->GetName());
			return;
		}
		
		const bool DoesCombatantExist = !HitTracedActors.Contains(CurrHitActor);
		if (DoesCombatantExist && ImplementsCombatInterface)
		{
			if(LogSlashLogic)
				UE_LOG(LogTemp, Log, TEXT("Trace Check Trace Success! %s"), *CurrHitActor->GetName());
			
			const bool IsBossUnit = false;
			const APawn* CharRef = Cast<APawn>(CurrHitActor);
			if (IsBossUnit)
			{
				// Process Body Part
			}
			else
			{
				HitTracedActors.Add(CurrHitActor);
				if (CharRef)
				{
					//for (int i = 0; i < 10 ; i++) // Simulating Spamming in network
					ProcessUnitDamage(CurrHitActor, ImpactPoint, 10.f, EDamageSource::Melee);
					//UE_LOG(LogTemp, Warning, TEXT("%s ProcessUnitDamage -> Actor: %s"), *UPXIINetworkBPLibrary::GetNetworkType(this), *GetNameSafe(CurrHitActor));
				}
				else
				{
					// Process Body Part
				}
			}
		}
		else if (ImplementsDamageableInterface)
		{
			APawn* PartOwner = IPxiiDamageableInterface::Execute_GetPawnOwner(CurrHitActor);
			if (!PartOwner) return;
			
			if (HitTracedActors.Contains(PartOwner))
			{
				return;
			}
			HitTracedActors.Add(PartOwner);
			IPxiiDamageableInterface::Execute_ApplyDamage(CurrHitActor, GetOwner(), 10, 0);
		}
		else
		{
			int32 newval = HitTracedActors.Contains(CurrHitActor) ? 0 : 1;
			int32 newval2 = ImplementsCombatInterface;
			UE_LOG(LogTemp, Log, TEXT("Loop Trace FAIL: %s %d %d"), *CurrHitActor->GetName(), newval, newval2);
		}
	}
}

void UPxiiCombatComponent::ProcessUnitDamage(AActor* TargetUnit, FVector HitLoc, float Damage, EDamageSource DamageSource)
{
	if (TargetUnit == nullptr || GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MISSING Target or Owner!"));
		return;
	}

	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MISSING Ability System Component!"));
		return;
	}
	
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(TargetUnit))
	{
		float PlayerDamage = 0;
		if (IsValid(GetOwner()) && GetOwner()->GetClass()->ImplementsInterface(UPxiiCombatInterface::StaticClass()))
		{
			//PlayerDamage = UPxiiCombatInterface::Execute_OnGetCurrentDamage(GetOwner());
			// TODO[ANY]: Interface Damage
			PlayerDamage = Damage;
		}
		/*
		const bool IsHeavyAttack = AbilitySystemComponent->HasMatchingGameplayTag(
				FGameplayTag::RequestGameplayTag(FName("Player.State.HeavyAttack")));
		const bool IsAbilityAttacking = AbilitySystemComponent->HasMatchingGameplayTag(
				FGameplayTag::RequestGameplayTag(FName("Player.State.IsAbilityAttacking")));
		*/
		const bool IsHeavyAttack = false;
		const bool IsAbilityAttacking = false;

		
		const float TotalDamage = PlayerDamage * (IsHeavyAttack ? 2 : 1);
			
		if (IsAbilityAttacking)
		{
			// Try Apply Reaction Damage
		}
		else
		{
			// This Should be sent to RPC Manager
			const UWorld* World = GetWorld();
			if (World)
			{
				if (UPxiiCombatRegistrySubsystem* CombatSubsystem = World->GetSubsystem<UPxiiCombatRegistrySubsystem>())
				{
					FDamageHit Hit;
					Hit.Target = TargetUnit;
					Hit.Damage = TotalDamage;
					Hit.Flags  = static_cast<uint8>(HitEffectType);
					Hit.HitCoord = HitLoc;
					Hit.DamageSource = DamageSource;

					CombatSubsystem->EnqueueDamage(GetOwner(), Hit);
					HasValidHit = true;
				}
				// Use for AOE
				// UCombatRegistryBPLibrary::ApplyAOEDamageEffect(GetOwner(), GetOwner(), HitLoc, 100.f, TotalDamage, EHitEffectType::Default, EDamageSource::None);
			}
			DisplayHitVfx.Broadcast(HitLoc);
		}
	}
}

void UPxiiCombatComponent::ProcessDPSDamage(AActor* TargetUnit, FVector HitLoc, const FDpsData& dpsData, EDamageSource DamageSource)
{
	if (TargetUnit == nullptr || GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MISSING Target or Owner!"));
		return;
	}

	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("MISSING Ability System Component!"));
		return;
	}

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(TargetUnit))
	{
		FDpsData data = dpsData;
	
		// This Should be sent to RPC Manager
		const UWorld* World = GetWorld();
		if (World)
		{
			if (UPxiiCombatRegistrySubsystem* CombatSubsystem = World->GetSubsystem<UPxiiCombatRegistrySubsystem>())
			{
				CombatSubsystem->ApplyDPSDamageEffect(GetOwner(), TargetUnit, dpsData.DamagePerPeriod, HitLoc,
					dpsData.Period, dpsData.Duration, dpsData.HitEffectType, DamageSource);
			}
		}
	}
}
