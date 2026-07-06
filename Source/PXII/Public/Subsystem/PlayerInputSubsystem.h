#pragma once
#include "Input/PxiiAbilityRouterManager.h"
#include "Input/PxiiInputBufferingManager.h"
#include "Input/PxiiInputMappingManager.h"
#include "Input/PxiiInputStateManager.h"
#include "AbilitySystemInterface.h"
#include "PlayerInputSubsystem.generated.h"

UCLASS()
class PXII_API UPlayerInputSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
private:
	
	UPROPERTY()
	TObjectPtr<UPxiiInputMappingManager> MappingManager;
	UPROPERTY()
	TObjectPtr<UPxiiInputBufferingManager> BufferManager;
	UPROPERTY()
	TObjectPtr<UPxiiInputStateManager> InputStateManager;
	UPROPERTY()
	TObjectPtr<UPxiiAbilityRouterManager> AbilityRouterManager;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void InitializeForPawn(APawn* Pawn);

	UFUNCTION(BlueprintPure)
	UPxiiInputMappingManager* GetInputMapManager();
	
	UFUNCTION(BlueprintPure)
	UPxiiInputBufferingManager* GetInputBufferManager();

	UFUNCTION(BlueprintPure)
	UPxiiInputStateManager* GetInputStateManager();
	
	UFUNCTION(BlueprintPure)
	UPxiiAbilityRouterManager* GetAbilityRouterManager();
};
