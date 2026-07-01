#pragma once
#include "Input/PxiiInputMappingManager.h"
#include "PlayerInputSubsystem.generated.h"

UCLASS()
class PXII_API UPlayerInputSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	TObjectPtr<UPxiiInputMappingManager> MappingManager;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

};
