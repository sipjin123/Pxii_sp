// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/PxiiUISubsystem.h"
#include "Engine/AssetManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Utility/PXIILogUtility.h"
#include "CommonUI/PxiiActivatableWidget.h"

UPxiiUISubsystem* UPxiiUISubsystem::Get(const UObject* WorldContextObject)
{
	if(GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

		return UGameInstance::GetSubsystem<UPxiiUISubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

void UPxiiUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

bool UPxiiUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if(!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UPxiiUISubsystem::RegisterBaseStack(UPxiiWidgetStackBase* InBaseStack)
{
	if(!InBaseStack)
	{
		return;
	}

	CachedStack = InBaseStack;
}

void UPxiiUISubsystem::PushSoftWidgetToStackAsync(const FGameplayTag & InStackTag, 
												  TSoftClassPtr<UPxiiActivatableWidget> InSoftWidgetClass, 
												  TFunction<void(EAsyncPushWidgetState, UPxiiActivatableWidget*)> InAsyncPushStateCallback)
{
	if(InSoftWidgetClass.IsNull())
	{
		return;
	}

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		InSoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[InSoftWidgetClass, InStackTag, this, InAsyncPushStateCallback]()
			{
				UClass* LoadedClass = InSoftWidgetClass.Get();
				if(!LoadedClass || !IsValid(CachedStack))
				{ 
					return;
				}

				UCommonActivatableWidgetContainerBase* FoundStack = CachedStack->FindStackByTag(InStackTag);

				if(!IsValid(FoundStack))
				{
					PXII_LOG(ELogCategory::UI, Warning, TEXT("No valid widget stack found for tag: %s"), *InStackTag.ToString());
					return;
				}

				UPxiiActivatableWidget* CreatedWidget = FoundStack->AddWidget<UPxiiActivatableWidget>(
					LoadedClass,
					[InAsyncPushStateCallback](UPxiiActivatableWidget& InCreatedWidgetInstance)
					{
						// On created, notify before push
						// Can be use to set up the widget before it is pushed to the stack
						InAsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &InCreatedWidgetInstance);
					}
				);

				// On pushed, notify after push
				// Can be use to set up the widget after it is pushed to the stack
				InAsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			}
		)
	);
}

TSoftClassPtr<UPxiiActivatableWidget> UPxiiUISubsystem::GetWidgetSoftClassByTag(const FGameplayTag InWidgetTag)
{
	if (DataMap)
	{
		if(!DataMap->UIClassesMap.Contains(InWidgetTag))
		{
			PXII_LOG(ELogCategory::UI, Warning, TEXT("[%s]: Widget class %s is not assigned in %s"), *ThisClass::StaticClass()->GetName(), *InWidgetTag.ToString(), *DataMap->GetName());
			return nullptr;
		}
	
		return DataMap->UIClassesMap.FindRef(InWidgetTag);
	}
	
	return nullptr;
}
