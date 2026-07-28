// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PxiiPlayerCombatComponent.h"
#include "AbilitySystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/PxiiTags.h"
#include "Enum/PxiiDamageType.h"

UPxiiPlayerCombatComponent::UPxiiPlayerCombatComponent()
{
}

// Called when the game starts
void UPxiiPlayerCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("CombatComponentStarted"));
	// ...
}

// Called every frame
void UPxiiPlayerCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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
