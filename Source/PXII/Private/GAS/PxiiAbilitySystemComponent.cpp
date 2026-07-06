// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PxiiAbilitySystemComponent.h"
#include "Data/PxiiAbilityData.h"
#include "Subsystem/PlayerInputSubsystem.h"

DEFINE_LOG_CATEGORY(LogTempAbilityComp);
void UPxiiAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPxiiAbilitySystemComponent::GrantAbilityByRow(FName RowName)
{
	if(!AbilityDataTable)return;

	const FPxiiAbilityData* Row = AbilityDataTable->FindRow<FPxiiAbilityData>(RowName,TEXT("GrantAbilityByRow"));

	if(!Row||!Row->AbilityClass)
	{
		return;	
	}

	FGameplayAbilitySpec Spec = FGameplayAbilitySpec(Row->AbilityClass,Row->Level,INDEX_NONE);
	Spec.GetDynamicSpecSourceTags().AddTag(Row->InputTag);
	
	GiveAbility(Spec);
}

void UPxiiAbilitySystemComponent::GrantAllAbilities()
{
	if(!AbilityDataTable)return;

	static const FString Context(TEXT("GrantAllAbilities"));

	TArray<FPxiiAbilityData*> Rows;
	AbilityDataTable->GetAllRows(Context,Rows);

	for(const FPxiiAbilityData* Row:Rows)
	{
		if(!Row||!Row->AbilityClass)continue;
		if (LogAbilityInit)
		{
			UE_LOG(LogTempAbilityComp,Warning,TEXT("GiveAbility: %s"),*Row->Name.ToString());
		}

		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(Row->AbilityClass,Row->Level,INDEX_NONE);
		Spec.GetDynamicSpecSourceTags().AddTag(Row->InputTag);
		
		GiveAbility(Spec);
	}
}

bool UPxiiAbilitySystemComponent::ConsumeBufferedInput(FGameplayTag InputTag)
{
	if (UPlayerInputSubsystem* InputSubsystem = GetPlayerInputSubsystem())
	{
		UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: Buffered Input"))
		return InputSubsystem->GetInputBufferManager()->ConsumeBufferedInput(InputTag);
	}

	return false;
}

bool UPxiiAbilitySystemComponent::HasBufferedInput(FGameplayTag InputTag) const
{
	if (UPlayerInputSubsystem* InputSubsystem = GetPlayerInputSubsystem())
	{
		UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: Buffered Input"))
		return InputSubsystem->GetInputBufferManager()->HasBufferedInput(InputTag);
	}

	return false;
}

bool UPxiiAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid())
	{
		return false;
	}

	FScopedAbilityListLock ActiveScopeLoc(*this);
	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			return TryActivateAbility(AbilitySpec.Handle);
		}
	}
	
	return false;
}

void UPxiiAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{

}

void UPxiiAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid())
	{
		return;
	}

	FScopedAbilityListLock ActiveScopeLoc(*this);
	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

UPlayerInputSubsystem* UPxiiAbilitySystemComponent::GetPlayerInputSubsystem() const
{
	const FGameplayAbilityActorInfo* Info = AbilityActorInfo.Get();
	if (!Info)
	{
		UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: Info Invalid"));
		return nullptr;
	}
	
	const APlayerController* PC = Info->PlayerController.Get();

	if (!PC)
	{
		UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: CAST GetController Invalid"));
		return nullptr;
	}

	if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
	{
		return LocalPlayer->GetSubsystem<UPlayerInputSubsystem>();
	}
	
	UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: LocalPlayer Invalid"));
	return nullptr;
}
