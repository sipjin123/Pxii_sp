// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PxiiAbilitySystemComponent.h"
#include "Data/PxiiAbilityData.h"
#include "Subsystem/PlayerInputSubsystem.h"
#include "Utility/PXIILogUtility.h"

DEFINE_LOG_CATEGORY(LogTempAbilityComp);

UPxiiAbilitySystemComponent::UPxiiAbilitySystemComponent()
{
	InputBlockTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Pxii.InputTag.Block")));
}

void UPxiiAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	OnAbilityEnded.AddUObject(this, &UPxiiAbilitySystemComponent::HandleAbilityEnded);
}

void UPxiiAbilitySystemComponent::GrantAbilityByRow(FName RowName)
{
	if(!AbilityData)
	{
		return;	
	}

	FAbilityData* Row = AbilityData->PlayerGrantedAbilities.FindByPredicate([&](const FAbilityData& Data)
	{
		return Data.AbilityID == RowName;
	});

	if(!Row||!Row->AbilityClass)
	{
		return;	
	}

	FGameplayAbilitySpec Spec = FGameplayAbilitySpec(Row->AbilityClass, Row->Level, INDEX_NONE);
	Spec.GetDynamicSpecSourceTags().AddTag(Row->InputTag);
	
	GiveAbility(Spec);
}

void UPxiiAbilitySystemComponent::GrantAllAbilities()
{
	if(!AbilityData)
	{
		return;	
	}

	static const FString Context(TEXT("GrantAllAbilities"));

	for(FAbilityData Row : AbilityData->PlayerGrantedAbilities)
	{
		if(!Row.AbilityClass)
		{
			continue;	
		}
		
		if (LogAbilityInit)
		{
			UE_LOG(LogTempAbilityComp,Warning,TEXT("GiveAbility: %s"),*Row.Name.ToString());
		}

		FGameplayAbilitySpec Spec = FGameplayAbilitySpec(Row.AbilityClass, Row.Level,INDEX_NONE);
		Spec.GetDynamicSpecSourceTags().AddTag(Row.InputTag);
		
		GiveAbility(Spec);
	}
}

void UPxiiAbilitySystemComponent::GrantAllPlayerEffects()
{
	if(!AbilityData)
	{
		return;	
	}

	if(AbilityData->PlayerGrantedAbilities.IsEmpty())
	{
		return;
	}

	static const FString Context(TEXT("GrantAllAbilities"));

	for(FGameplayEffectData Row : AbilityData->PlayerGrantedEffect)
	{
		if(!Row.EffectClass)
		{
			continue;	
		}
		
		if (LogAbilityInit)
		{
			UE_LOG(LogTempAbilityComp, Warning,TEXT("Give Effect: %s"),*Row.Name.ToString());
		}

		FGameplayEffectContextHandle context = MakeEffectContext();
		context.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(Row.EffectClass, Row.Level, context);
		if(SpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

bool UPxiiAbilitySystemComponent::IsInputBlocked() const
{
	return HasAnyMatchingGameplayTags(InputBlockTags);
}

bool UPxiiAbilitySystemComponent::ConsumeBufferedInput(FGameplayTag InputTag)
{
	if (UPlayerInputSubsystem* InputSubsystem = GetPlayerInputSubsystem())
	{
		UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: Checking Buffered Input"))
		return InputSubsystem->GetInputBufferManager()->ConsumeBufferedInput(InputTag);
	}

	return false;
}

bool UPxiiAbilitySystemComponent::ConsumeAndQueueInput(FGameplayTag InputTag)
{
	if (UPlayerInputSubsystem* InputSubsystem = GetPlayerInputSubsystem())
	{
		if(InputSubsystem->GetInputBufferManager()->HasQueuedInput(InputTag))
		{
			PXII_LOG(ELogCategory::Ability, Log, TEXT("DZ_LOG:: Queue Failed"));
			return false;
		}

		if(InputSubsystem->GetInputBufferManager()->HasBufferedInput(InputTag))
		{
			bool result = InputSubsystem->GetInputBufferManager()->ConsumeAndQueueBufferedInput(InputTag);
			return result;
		}
		
		return false;
	}

	PXII_LOG(ELogCategory::Ability, Log, TEXT("DZ_LOG:: Subsystem Failed"));
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

bool UPxiiAbilitySystemComponent::CanQueueInput(FGameplayTag InputTag) const
{
	if (UPlayerInputSubsystem* InputSubsystem = GetPlayerInputSubsystem())
	{
		UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: Checking Buffered Input"))
		return !InputSubsystem->GetInputBufferManager()->HasQueuedInput(InputTag);
	}

	return false;
}

bool UPxiiAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid())
	{
		return false;
	}

	bool bActivated = false;
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for(FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			bActivated |= TryActivateAbility(AbilitySpec.Handle);
		}
	}
	
	return bActivated;
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

void UPxiiAbilitySystemComponent::HandleAbilityEnded(const FAbilityEndedData& EndData)
{
	if (UPlayerInputSubsystem* InputSubsystem = GetPlayerInputSubsystem())
	{
		FGameplayTag queuedTag = InputSubsystem->GetInputBufferManager()->ConsumeQueuedInput();
		if(queuedTag != FGameplayTag::EmptyTag)
		{
			AbilityInputTagPressed(queuedTag);
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
