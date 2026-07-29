#include "PxiiAimComponent.h"
#include "Utility/PXIILogUtility.h"

void UPxiiAimComponent::BeginPlay()
{
	Super::BeginPlay();

	AimTimeline = NewObject<UTimelineComponent>(this);
	AimTimeline->AddInterpFloat(AimCurve,FOnTimelineFloatStatic::CreateUObject(this, &UPxiiAimComponent::UpdateAim));
	AimTimeline->RegisterComponent();
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if(ASC)
	{
		FGameplayTag adsTag = FGameplayTag::RequestGameplayTag("Pxii.Combat.State.ADS");
		ASC->RegisterGameplayTagEvent( adsTag,	EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UPxiiAimComponent::OnAimTagChanged);
		PXII_LOG(ELogCategory::Controls, Log, TEXT("DZ_LOG:: Component BEGIN"));		
	}
}

void UPxiiAimComponent::StartAim_Implementation()
{
	AimTimeline->PlayFromStart();
}

void UPxiiAimComponent::StopAim_Implementation()
{
	AimTimeline->Reverse();
}

void UPxiiAimComponent::OnAimTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		StartAim();
	}
	else
	{
		StopAim();
	}
}
