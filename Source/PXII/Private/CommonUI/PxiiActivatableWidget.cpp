#include "CommonUI\PxiiActivatableWidget.h"

#include "Input/CommonUIInputTypes.h"

void UPxiiActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
}

void UPxiiActivatableWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
}

void UPxiiActivatableWidget::NativeDestruct()
{
	for (FUIActionBindingHandle BindingHandle : BindingHandles)
	{
		if (BindingHandle.IsValid())
		{
			BindingHandle.Unregister();
		}
	}
	BindingHandles.Empty();
	
	Super::NativeDestruct();
}

void UPxiiActivatableWidget::RegisterBinding(FDataTableRowHandle InputAction, const bool bShowInActionBar,
	FInputActionExecutedDelegate NormalCallback, 
	FInputActionHoldStartedDelegate OnHoldStarted,
	FInputActionHoldProgressDelegate OnHoldProgress,
	FInputActionHoldReleasedDelegate OnHoldReleased,
	FInputActionBindingHandle& BindingHandle)
{
	if (InputAction.IsNull())
	{
		return;	
	}
	
	FBindUIActionArgs Args(
		InputAction, 
		bShowInActionBar,
		FSimpleDelegate::CreateWeakLambda(
			this, 
			[InputAction, NormalCallback]()
			{
				if (NormalCallback.IsBound())
				{
					NormalCallback.ExecuteIfBound();
				}
			}
		)
	);
	
	if (OnHoldStarted.IsBound())
	{
		Args.OnHoldActionPressed.BindWeakLambda(
		   this,
		   [OnHoldStarted]
		   {
			   OnHoldStarted.ExecuteIfBound();
		   }
	   );
	}
	
	if (OnHoldProgress.IsBound())
	{
		Args.OnHoldActionProgressed.BindWeakLambda(
		   this,
		   [OnHoldProgress](const float Progress)
		   {
			   OnHoldProgress.ExecuteIfBound(Progress);
		   }
	   );
	}
	
	if (OnHoldReleased.IsBound())
	{
		Args.OnHoldActionReleased.BindWeakLambda(
		   this,
		   [OnHoldReleased]()
		   {
			   OnHoldReleased.ExecuteIfBound();
		   }
	   );
	}
	
	BindingHandle.Handle = RegisterUIActionBinding(Args);
			
	BindingHandles.Add(BindingHandle.Handle);
}

void UPxiiActivatableWidget::UnregisterBinding(FInputActionBindingHandle BindingHandle)
{
	if (BindingHandle.Handle.IsValid())
	{
		BindingHandle.Handle.Unregister();
		BindingHandles.Remove(BindingHandle.Handle);
	}
}

void UPxiiActivatableWidget::UnregisterAllBindings()
{
	for (FUIActionBindingHandle BindingHandle : BindingHandles)
	{
		BindingHandle.Unregister();
	}
	BindingHandles.Empty();
}

APxiiPlayerController* UPxiiActivatableWidget::GetPlayerController() const
{
	return Cast<APxiiPlayerController>(GetOwningPlayer());
}

APxiiCharacter* UPxiiActivatableWidget::GetPlayerPawn() const
{
	return Cast<APxiiCharacter>(GetOwningPlayerPawn());
}
