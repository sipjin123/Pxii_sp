// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/AsyncActions/WaitAbilityBlockedTagsAsync.h"
#include "AbilitySystemComponent.h"
#include "Utility/PXIILogUtility.h"

UWaitAbilityBlockedTagsAsync* UWaitAbilityBlockedTagsAsync::WaitForAbilityBlockedTags(AActor* TargetActor,
                                                                                      TSubclassOf<UPxiiGameplayAbilityBase> AbilityClass, bool bOnlyTriggerOnce)
{
	UWaitAbilityBlockedTagsAsync* NewObj = NewObject<UWaitAbilityBlockedTagsAsync>();
	NewObj->SetAbilityActor(TargetActor);
	NewObj->CachedAbilityClass = AbilityClass;
	NewObj->bOnlyTriggerOnce = bOnlyTriggerOnce;
	
	return NewObj;
}

void UWaitAbilityBlockedTagsAsync::Activate()
{
	Super::Activate();
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC || !CachedAbilityClass)
	{
		PXII_LOG(ELogCategory::Ability, Warning, TEXT("[%s]: Invalid ASC or ability class"), *ThisClass::StaticClass()->GetName());
		EndAction();
		return;
	}
	
	FGameplayAbilitySpec* FoundSpec = ASC->FindAbilitySpecFromClass(CachedAbilityClass);
	if (!FoundSpec || !FoundSpec->Ability)
	{
		PXII_LOG(ELogCategory::Ability, Warning, TEXT("[%s]: No ability found"), *ThisClass::StaticClass()->GetName());
		EndAction();
		return;
	}
	
	CachedBlockedTags.Reset();
	if (const UPxiiGameplayAbilityBase* AbilityInstance = Cast<UPxiiGameplayAbilityBase>(FoundSpec->GetPrimaryInstance()))
	{
		CachedBlockedTags.AppendTags(AbilityInstance->GetActivationBlockedTags());
	}
	else
	{
		const UPxiiGameplayAbilityBase* Ability = Cast<UPxiiGameplayAbilityBase>(FoundSpec->Ability);
		if (!Ability)
		{
			EndAction();
			return;
		}
		
		CachedBlockedTags.AppendTags(Ability->GetActivationBlockedTags());
	}
	
	if (CachedBlockedTags.IsEmpty())
	{
		EndAction();
		return;
	}
	
	for (const FGameplayTag& Tag : CachedBlockedTags)
	{
		if (ASC->HasMatchingGameplayTag(Tag))
		{
			OnBlockedTagAdded.Broadcast(Tag, true);
			if (bOnlyTriggerOnce)
			{
				EndAction();
				return;
			}
		}
		
		FDelegateHandle Handle = ASC->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnTagChanged);
		
		RegisteredTagHandles.Add(Tag, Handle);
	}
}

void UWaitAbilityBlockedTagsAsync::EndAction()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (const auto& TagHandle : RegisteredTagHandles)
		{
			ASC->RegisterGameplayTagEvent(TagHandle.Key, EGameplayTagEventType::NewOrRemoved).Remove(TagHandle.Value);
		}
	}
	RegisteredTagHandles.Empty();
	CachedBlockedTags.Reset();
	
	Super::EndAction();
}

void UWaitAbilityBlockedTagsAsync::OnTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (!ShouldBroadcastDelegates())
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!ASC)
	{
		PXII_LOG(ELogCategory::Ability, Warning, TEXT("[%s]: Invalid ASC"), *ThisClass::StaticClass()->GetName());
		EndAction();
		return;
	}
	
	if (NewCount > 0)
	{
		OnBlockedTagAdded.Broadcast(CallbackTag, true);
	}
	else
	{
		const bool bIsStillBlocking = ASC->HasAnyMatchingGameplayTags(CachedBlockedTags);
		OnBlockedTagRemoved.Broadcast(CallbackTag, bIsStillBlocking);
	}
	
	if (bOnlyTriggerOnce)
	{
		EndAction();
	}
}
