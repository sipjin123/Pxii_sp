// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/AsyncActions/WaitAbilityRequiredTagsAsync.h"

#include "AbilitySystemComponent.h"
#include "Utility/PXIILogUtility.h"

UWaitAbilityRequiredTagsAsync* UWaitAbilityRequiredTagsAsync::WaitForAbilityRequiredTags(
	UObject* WorldContextObject, AActor* TargetActor, TSubclassOf<UPxiiGameplayAbilityBase> AbilityClass,
	bool bOnlyTriggerOnce)
{
	UWaitAbilityRequiredTagsAsync* NewObj = NewObject<UWaitAbilityRequiredTagsAsync>();
	NewObj->SetAbilityActor(TargetActor);
	NewObj->CachedAbilityClass = AbilityClass;
	NewObj->bOnlyTriggerOnce = bOnlyTriggerOnce;
	NewObj->RegisterWithGameInstance(WorldContextObject);
	
	return NewObj;
}

void UWaitAbilityRequiredTagsAsync::Activate()
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
	
	CachedRequiredTags.Reset();
	if (const UPxiiGameplayAbilityBase* AbilityInstance = Cast<UPxiiGameplayAbilityBase>(FoundSpec->GetPrimaryInstance()))
	{
		CachedRequiredTags.AppendTags(AbilityInstance->GetActivationRequiredTags());
	}
	else
	{
		const UPxiiGameplayAbilityBase* Ability = Cast<UPxiiGameplayAbilityBase>(FoundSpec->Ability);
		if (!Ability)
		{
			EndAction();
			return;
		}
		
		CachedRequiredTags.AppendTags(Ability->GetActivationRequiredTags());
	}
	
	if (CachedRequiredTags.IsEmpty())
	{
		bIsCurrentTagsMet = true;
		OnRequiredTagsMet.Broadcast(FGameplayTag::EmptyTag, FGameplayTagContainer::EmptyContainer);
		EndAction();
		return;
	}
	
	const FGameplayTagContainer InitialMissingTag = GetMissingRequiredTags(ASC);
	if (InitialMissingTag.IsEmpty())
	{
		bIsCurrentTagsMet = true;
		OnRequiredTagsMet.Broadcast(FGameplayTag::EmptyTag, FGameplayTagContainer::EmptyContainer);
		if (bOnlyTriggerOnce)
		{
			EndAction();
			return;
		}
	}
	else
	{
		bIsCurrentTagsMet = false;
		OnRequiredTagMissing.Broadcast(FGameplayTag::EmptyTag, InitialMissingTag);
	}
	
	for (const FGameplayTag& Tag : CachedRequiredTags)
	{
		FDelegateHandle TagHandle = ASC->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnTagChanged);
		
		RegisteredTagHandles.Add(Tag, TagHandle);
	}
}

void UWaitAbilityRequiredTagsAsync::EndAction()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (const auto& TagHandle : RegisteredTagHandles)
		{
			ASC->RegisterGameplayTagEvent(TagHandle.Key, EGameplayTagEventType::NewOrRemoved).Remove(TagHandle.Value);
		}
	}
	RegisteredTagHandles.Empty();
	CachedRequiredTags.Reset();
	
	Super::EndAction();
}

void UWaitAbilityRequiredTagsAsync::OnTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
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
	
	const FGameplayTagContainer MissingTags = GetMissingRequiredTags(ASC);
	const bool bIsNowMet = MissingTags.IsEmpty();
	
	if (bIsNowMet && !bIsCurrentTagsMet)
	{
		bIsCurrentTagsMet = true;
		OnRequiredTagsMet.Broadcast(CallbackTag, FGameplayTagContainer::EmptyContainer);
		
		if (bOnlyTriggerOnce)
		{
			EndAction();
		}
	}
	else if (!bIsNowMet && bIsCurrentTagsMet)
	{
		bIsCurrentTagsMet = false;
		OnRequiredTagMissing.Broadcast(CallbackTag, MissingTags);
	}
}

FGameplayTagContainer UWaitAbilityRequiredTagsAsync::GetMissingRequiredTags(class UAbilitySystemComponent* ASC) const
{
	FGameplayTagContainer MissingTags;
	for (const FGameplayTag& Tag : CachedRequiredTags)
	{
		if (!ASC->HasMatchingGameplayTag(Tag))
		{
			MissingTags.AddTag(Tag);
		}
	}
	return MissingTags;
}
