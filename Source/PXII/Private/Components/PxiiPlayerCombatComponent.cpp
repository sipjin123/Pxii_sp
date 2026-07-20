// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PxiiPlayerCombatComponent.h"

#include "AbilitySystemComponent.h"
#include "DrawDebugHelpers.h"
#include "GameplayTagContainer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/PxiiTags.h"
#include "Enum/PxiiDamageType.h"
#include "Subsystem/PxiiCombatRegistrySubsystem.h"

UPxiiPlayerCombatComponent::UPxiiPlayerCombatComponent()
{
}

// Called when the game starts
void UPxiiPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("CombatComponentStarted"));
	// ...
	CharacterRef = Cast<APxiiCharacter>(GetOwner());
	ActorsToIgnore.Add(GetOwner());
	//DrawDebugTrace = EDrawDebugTrace::ForDuration;
	//UseDepthSlash = true;
	AbilitySystemComponent = CharacterRef->GetAbilitySystemComponent();

	// DEV: Code Override, remove later
	//LogSlashLogic = false;
}

// Called every frame
void UPxiiPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPxiiPlayerCombatComponent::InitializeHitTrace(FVector SocketLoc)
{
	HitTracedActors.Empty();
	TraceIndex = 0;
	HasValidHit = false;
	HasHitReaction = false;
	CharacterRef->IsAttacking = true;
	//CharacterRef->SetIsWeaponMeleeEquipped(true);
}

void UPxiiPlayerCombatComponent::ProcessHitTrace(FVector SocketLoc)
{
	// Only process trace logic when a start and end point is generated
	if (TraceIndex > 0)
	{
		ProcessHitTraceLogic(PreviousSocketLocation, SocketLoc);
	}
	TraceIndex++;
	PreviousSocketLocation = SocketLoc;
}

void UPxiiPlayerCombatComponent::ProcessHitTraceLogic(FVector StartLoc, FVector EndLoc)
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
	
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetOwner(),
		StartLoc, EndLoc, Radius, ObjectTypes,
		false,
		ActorsToIgnore, DrawDebugTrace, OutHits, true,
		GetOwner()->HasAuthority() ? FColor::Green : FColor::Cyan,
		GetOwner()->HasAuthority() ? FColor::Red :FColor::Orange, TraceDuration
	);
	SlashDataArray = OutHits;

	// Handles depth slash logic and adds to SlashDataArray
	if (UseDepthSlash)
	{
		ProcessDepthSlash(EndLoc);
	}
	
	FinalizeHitTraceLogic();
}

void UPxiiPlayerCombatComponent::EndHitTrace()
{
	CharacterRef->IsAttacking = false;
}

void UPxiiPlayerCombatComponent::ProcessDepthSlash(FVector EndLoc)
{
	// Query Setup
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

	constexpr float DepthRadius = 45.f;
	TArray<FHitResult> DepthOutHits;
	FVector HandLocation = CharacterRef->GetMesh()->GetSocketLocation("palm_r_Socket");
	UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetOwner(),
		HandLocation,
		EndLoc,
		DepthRadius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		DrawDebugTrace,
		DepthOutHits,
		true,
		FColor::Blue,FColor::Red, TraceDuration
	);

	for (FHitResult OutHitParam : DepthOutHits)
	{
		SlashDataArray.Add(OutHitParam);
	}
}

void UPxiiPlayerCombatComponent::FinalizeHitTraceLogic()
{
	//if (LogSlashLogic)
	//	UE_LOG(LogTemp, Warning, TEXT("%s Slash Trace: Finalize Index:%d"), *UPXIINetworkBPLibrary::GetNetworkType(this), TraceIndex);
	/*
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn)
	{
		return;
	}

	const bool bIsLocalPlayer = Pawn->IsLocallyControlled();
	const bool bIsServer = Pawn->HasAuthority();

	if (!bIsLocalPlayer && !bIsServer)
	{
		return;
	}*/
	
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

		if (Cast<APxiiCharacter>(OutHitParam.GetActor()))
		{
			DrawDebugSphere(GetWorld(), OutHitParam.ImpactPoint, 100.f, // Radius
				12, FColor::Emerald, false, // Segments (visual quality) // Color // bPersistentLines
				10.f,0, 1.f // LifeTime // DepthPriority // Thickness
			);
		}
		
		const bool ImplementsCombatInterface = CurrHitActor->GetClass()->ImplementsInterface(UPxiiCombatInterface::StaticClass());
		if (ImplementsCombatInterface && !HitTracedActors.Contains(CurrHitActor))
		{
			//const bool IsBossUnit = UPxiiCombatInterface::Execute_IsBossUnit(CurrHitActor);
			const bool IsBossUnit = false;
			const ACharacter* CharRef = Cast<ACharacter>(CurrHitActor);
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
					ProcessUnitDamage(CurrHitActor, ImpactPoint);
					//UE_LOG(LogTemp, Warning, TEXT("%s ProcessUnitDamage -> Actor: %s"), *UPXIINetworkBPLibrary::GetNetworkType(this), *GetNameSafe(CurrHitActor));
				}
				else
				{
					// Process Body Part
				}
			}
		}
	}
}

void UPxiiPlayerCombatComponent::ProcessUnitDamage(AActor* TargetUnit, FVector HitLoc)
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
			PlayerDamage = 10.f;
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
					Hit.Flags  = 1;
					Hit.HitCoord = HitLoc;
					Hit.DamageSource = EDamageSource::Melee;

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

void UPxiiPlayerCombatComponent::GrantYinYang(EPxiiYinYangType Type, float Amount, bool bIsPercent)
{
	if (!AbilitySystemComponent || !GE_YinYang) return;

	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GE_YinYang, 1.0f, Context);

	if (!SpecHandle.IsValid()) return;

	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
	if (!Spec) return;

	Spec->SetSetByCallerMagnitude(TAG_Data_YinYangType, static_cast<float>(Type));
	Spec->SetSetByCallerMagnitude(TAG_Data_Amount, Amount);
	Spec->SetSetByCallerMagnitude(TAG_Data_IsPercent, bIsPercent ? 1.f : 0.f);

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
}
