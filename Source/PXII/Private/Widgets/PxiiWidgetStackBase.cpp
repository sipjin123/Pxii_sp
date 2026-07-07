// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PxiiWidgetStackBase.h"
#include "Components/NamedSlot.h"

UUserWidget* UPxiiWidgetStackBase::PushWidgetToStack(TSubclassOf<UUserWidget> WidgetClass, FGameplayTag InStackTag, bool bActivateOnCreate)
{
	if (!WidgetClass || RegisteredWidgetStack.IsEmpty())
	{
		return nullptr;
	}

	// Create a new widget instance of the specified class
	UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), WidgetClass);
	if (!NewWidget) 
	{
		return nullptr;
	}

	UNamedSlot* Stack = Cast<UNamedSlot>(FindStackByTag(InStackTag));
	Stack->SetContent(NewWidget);

	if (bActivateOnCreate)
	{
		OnWidgetActivated(NewWidget);
	}

	return NewWidget;
}

void UPxiiWidgetStackBase::PopWidgetFromStack(FGameplayTag InStackTag)
{
	if (RegisteredWidgetStack.IsEmpty())
	{
		return;
	}

	UNamedSlot* Stack = Cast<UNamedSlot>(FindStackByTag(InStackTag));
	UWidget* WidgetToRemove = Stack->GetChildAt(Stack->GetChildrenCount() - 1);
	WidgetToRemove->RemoveFromParent();
}

void UPxiiWidgetStackBase::PopAllWidget()
{
}

UUserWidget* UPxiiWidgetStackBase::GetTopMostWidget() const
{
	return nullptr;
}

UWidget* UPxiiWidgetStackBase::FindStackByTag(const FGameplayTag& InStackTag) const
{
	if (RegisteredWidgetStack.Contains(InStackTag))
	{
		return RegisteredWidgetStack.FindRef(InStackTag);
	}

	return nullptr;
}

void UPxiiWidgetStackBase::RegisterWidgetStack(UPARAM(meta = (Categories = "Pxii.UI.WidgetStack")) FGameplayTag InStackTag, UWidget* InWidgetStack)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStack.Contains(InStackTag))
		{
			RegisteredWidgetStack.Add(InStackTag, InWidgetStack);
		}
	}
}