// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PxiiGameplayAbilityBase.h"


UPxiiGameplayAbilityBase::UPxiiGameplayAbilityBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Pxii.Input.Mode.UI")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("InputMode.Menu")));
}

UMoverComponent* UPxiiGameplayAbilityBase::GetMoverComponent()
{
	AActor* Actor = GetAvatarActorFromActorInfo();
	UMoverComponent* Mover = Actor->FindComponentByClass<UMoverComponent>();

	return Mover;
}
