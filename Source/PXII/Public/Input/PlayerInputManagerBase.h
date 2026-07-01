#pragma once
#include "EnhancedInputSubsystems.h"
#include "PlayerInputManagerBase.generated.h"

UCLASS(Abstract)
class PXII_API UPlayerInputManagerBase : public UObject
{
	GENERATED_BODY()
	
public:

	virtual void Initialize(ULocalPlayer* Owner)
	{
		LocalPlayer = Owner;
	}

protected:

	UPROPERTY()
	TObjectPtr<ULocalPlayer> LocalPlayer;

	UEnhancedInputLocalPlayerSubsystem* GetInputSubsystem() const;
	
};
