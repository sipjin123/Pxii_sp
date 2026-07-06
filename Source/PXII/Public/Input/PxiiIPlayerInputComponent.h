#pragma once
#include "EnhancedInputComponent.h"
#include "PxiiPlayerInputConfig.h"
#include "PxiiIPlayerInputComponent.generated.h"

UCLASS()
class PXII_API UPxiiIPlayerInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType, typename HoldingFuncType>
	void BindAbilityActions(const UPxiiPlayerInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc, HoldingFuncType HoldingFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType, typename HoldingFuncType>
void UPxiiIPlayerInputComponent::BindAbilityActions(const UPxiiPlayerInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc, HoldingFuncType HoldingFunc)
{
	check(InputConfig);

	for (const FXIIInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Canceled, Object, ReleasedFunc, Action.InputTag);
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}

			if (HoldingFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Ongoing, Object, HoldingFunc, Action.InputTag);
			}
		}
	}
}

