// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/PxiiCombatRegistrySubsystem.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "DrawDebugHelpers.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Enum/PxiiDamageType.h"

class UGameplayEffect;
class IAbilitySystemInterface;

DEFINE_LOG_CATEGORY(LogCombatRegistrySubsystem);
void UPxiiCombatRegistrySubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
}

void UPxiiCombatRegistrySubsystem::EnqueueDamage(AActor* Source, const FDamageHit& Hit)
{
	// This is the function that accepts damage requests
	if (!Source || !Hit.Target.IsValid())
	{
		return;
	}

	FQueuedDamage& Entry = DamageQueue.Emplace_GetRef();
	Entry.Source = Source;
	Entry.HitData = Hit;

	if (bLogFlow)
	{
		const UEnum* Enum = StaticEnum<EHitEffectType>();
		const FString EName = Enum->GetNameStringByValue((int64)Hit.Flags);
		const FString EDName = Enum->GetNameStringByValue((int64)Hit.DamageSource);

		UE_LOG(LogTemp, Log,
			TEXT("[CRS] -- Enqueued: %s -> %s | Damage=%.1f | Flag=%s | Source=%s"),
			*GetNameSafe(Source),
			*GetNameSafe(Hit.Target.Get()),
			Hit.Damage, *EName, *EDName
		);
	}
	
	EnsureProcessing();
}

void UPxiiCombatRegistrySubsystem::EnsureProcessing()
{
	if (bLogFlow)
		UE_LOG(LogTemp, Warning, TEXT("[CRS] START Queue -------------------------------------- "));
	
	if (bProcessingActive)
	{
		return;
	}

	if (!GetWorld())
	{
		return;
	}

	bProcessingActive = true;
	constexpr float FrameTimerRate = 1.f / 60.f;
	
	if (bLogFlow)
		UE_LOG(LogTemp, Warning, TEXT("[CRS] TIMER START -------------------------------------- "));
	
	GetWorld()->GetTimerManager().SetTimer(ProcessTimer, this, &UPxiiCombatRegistrySubsystem::ManualTick,
		FrameTimerRate,   // every frame
		true);
}

void UPxiiCombatRegistrySubsystem::ManualTick()
{
	if (bLogFlow)
		UE_LOG(LogTemp, Warning, TEXT("[CRS] Tick START -------------------------------------- "));

	if (DamageQueue.Num() == 0)
	{
		return;
	}
	
	TMap<AActor*, int32> SourceRpcCount;
	SourceRpcCount.Reset();
	int32 Processed = 0;

	while (DamageQueue.Num() > 0 && Processed < MaxDamagePerTick)
	{
		FQueuedDamage Entry = DamageQueue[0];
		DamageQueue.RemoveAt(0);

		AActor* Source = Entry.Source.Get();
		AActor* Target = Entry.HitData.Target.Get();

		if (!Source || !Target) 
		{
			continue;
		}

		if (bLogFlow)
			UE_LOG(LogTemp, Warning, TEXT("[CRS] %d ---- PROCESS: %s -> %s | Damage=%.1f Flags=%u Source=%u"), CurrentTickIndex,
				*GetNameSafe(Source), *GetNameSafe(Target), Entry.HitData.Damage, Entry.HitData.Flags, Entry.HitData.DamageSource);
		
		// 🔹 Check per-source RPC limit
		int32& RpcCount = SourceRpcCount.FindOrAdd(Source);

		const bool bAllowCue = (RpcCount < 2);
		if (!bAllowCue)
		{
			UE_LOG(LogTemp, Warning, TEXT("[CRS] %d -BREAK BREAK BREAK BREAK"), CurrentTickIndex);
			break;
		}
		
		ProcessDamageRegistry(Entry);
		RpcCount++;
		Processed++;
	}
	
	// 🔴 Nothing left → stop timer
	if (DamageQueue.Num() == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ProcessTimer);
		bProcessingActive = false;
	}
	CurrentTickIndex++;

	if (bLogFlow)
		UE_LOG(LogTemp, Warning, TEXT("[CRS] Tick END -------------------------------------- "));
}

void UPxiiCombatRegistrySubsystem::ProcessDamageRegistry(const FQueuedDamage& DamageEntry)
{
	const UWorld* World = GetWorld();
	if (World)
	{
		IAbilitySystemInterface* SourceASI = Cast<IAbilitySystemInterface>(DamageEntry.Source);
		IAbilitySystemInterface* TargetASI = Cast<IAbilitySystemInterface>(DamageEntry.HitData.Target.Get());

		if (SourceASI && TargetASI)
		{
			if (bLogFlow)
				UE_LOG(LogTemp, Warning, TEXT("[CRS] %d ---- Process Damage, Remain {%d}"), CurrentTickIndex, DamageQueue.Num());
			EHitEffectType EffectType = static_cast<EHitEffectType>(DamageEntry.HitData.Flags);
			ApplySingleDamageEffect(
				DamageEntry.Source.Get(),
				DamageEntry.HitData.Target.Get(),
				DamageEntry.HitData.Damage,
				DamageEntry.HitData.HitCoord,
				EffectType,
				DamageEntry.HitData.DamageSource
			);

			if (TraceOrigin)
			{
				DrawDebugSphere(World, DamageEntry.HitData.HitCoord, 25.f, // Radius
							12, FColor::Blue, false, // Segments (visual quality) // Color // bPersistentLines
							5.f, 0, 1.f // LifeTime // DepthPriority // Thickness
						);
			}
		}
	}
}

void UPxiiCombatRegistrySubsystem::ApplySingleDamageEffect(AActor* Source, AActor* Target, float Damage, FVector HitCoords, EHitEffectType HitEffectType, EDamageSource DamageSource)
{
	if (!Source->HasAuthority()) return;
	
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Source);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!SourceASC || !TargetASC) return;
	
	const UWorld* World = Source->GetWorld();
	if (!World) return;
	
	//const UPxiiCombatRegistrySubsystem* CombatRegistry = World->GetSubsystem<UPxiiCombatRegistrySubsystem>();
	//if (!CombatRegistry) return;
	
	const TSubclassOf<UGameplayEffect> DamageEffect = GetGenericDamageEffect(); // CombatRegistry->
	if (!DamageEffect) return;

	// ---------------------------------------------

	// Create effect context
	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(SourceASC);
	EffectContext.AddOrigin(HitCoords);
	//FHitResult Hit;
	//Hit.ElementIndex = static_cast<uint8>(HitEffectType);
	//EffectContext.AddHitResult(Hit);

	const FGameplayEffectSpecHandle ParrySpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect, 1.0f, EffectContext);
	if (ParrySpecHandle.IsValid())
	{
		// Add the magnitude value as a tag with payload.
		const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Combat.Damage"));
		ParrySpecHandle.Data->SetSetByCallerMagnitude(DamageTag, Damage);

		const FGameplayTag HitTypeTag = FGameplayTag::RequestGameplayTag(FName("Combat.HitType"));
		ParrySpecHandle.Data->SetSetByCallerMagnitude(HitTypeTag, static_cast<float>(HitEffectType));

		const FGameplayTag DamageSourceTag = FGameplayTag::RequestGameplayTag(FName("Combat.DamageSource"));
		ParrySpecHandle.Data->SetSetByCallerMagnitude(DamageSourceTag, static_cast<float>(DamageSource));
		
		// Apply to Target
		if (bLogFlow)
			UE_LOG(LogTemp, Warning, TEXT("[CRS] %d -------------------------------------- Damage Applied, Remain {%d} INDEX: {%d} --  {%d}"), CurrentTickIndex, DamageQueue.Num(), DamageSource, HitEffectType);
		SourceASC->ApplyGameplayEffectSpecToTarget(*ParrySpecHandle.Data.Get(), TargetASC);

		/*
		if (Target->GetClass()->ImplementsInterface(UICombatant::StaticClass()))
		{
			// TODO: Logic for determining intensity here
			int32 IntensityValue = 1;
			switch (HitEffectType)
			{
				case EHitEffectType::Flinch:
					IICombatant::Execute_ApplyFlinch(Target);
					break;
				case EHitEffectType::Knockback:
					IICombatant::Execute_ApplyKnockback(Target, IntensityValue);
					break;
				case EHitEffectType::Knockdown:
					IICombatant::Execute_ApplyKnockdown(Target, IntensityValue);
					break;
			}
		}*/
	}
}

void UPxiiCombatRegistrySubsystem::ApplyDPSDamageEffect(AActor* Source, AActor* Target, float Damage, FVector HitCoords,
	float period, float duration, EHitEffectType HitEffectType, EDamageSource DamageSource)
{
	
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Source);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!SourceASC || !TargetASC) return;
	
	const UWorld* World = Source->GetWorld();
	if (!World) return;
	
	//const UPxiiCombatRegistrySubsystem* CombatRegistry = World->GetSubsystem<UPxiiCombatRegistrySubsystem>();
	//if (!CombatRegistry) return;
	
	const TSubclassOf<UGameplayEffect> DamageEffect = GetGenericDPSEffect(); // CombatRegistry->
	if (!DamageEffect) return;

	// ---------------------------------------------

	// Create effect context
	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(SourceASC);
	EffectContext.AddOrigin(HitCoords);
	
	const FGameplayEffectSpecHandle DPSSpec = SourceASC->MakeOutgoingSpec(DamageEffect, 1.0f, EffectContext);

	if (FGameplayEffectSpec* Spec = DPSSpec.Data.Get())
	{
		Spec->SetDuration(duration, true);

		// Modify the period
		Spec->Period = period;

		// Add the magnitude value as a tag with payload.
		const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Combat.Damage"));
		DPSSpec.Data->SetSetByCallerMagnitude(DamageTag, Damage);

		const FGameplayTag HitTypeTag = FGameplayTag::RequestGameplayTag(FName("Combat.HitType"));
		DPSSpec.Data->SetSetByCallerMagnitude(HitTypeTag, static_cast<float>(HitEffectType));

		const FGameplayTag DamageSourceTag = FGameplayTag::RequestGameplayTag(FName("Combat.DamageSource"));
		DPSSpec.Data->SetSetByCallerMagnitude(DamageSourceTag, static_cast<float>(DamageSource));
		
		// Apply to Target
		if (bLogFlow)
			UE_LOG(LogTemp, Warning, TEXT("[CRS] %d -------------------------------------- Damage Applied, Remain {%d} INDEX: {%d} --  {%d}"), CurrentTickIndex, DamageQueue.Num(), DamageSource, HitEffectType);

		SourceASC->ApplyGameplayEffectSpecToTarget(*DPSSpec.Data.Get(), TargetASC);
	}
}
