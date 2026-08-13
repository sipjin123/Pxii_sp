// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PxiiCharSimulatorComponent.generated.h"


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Exploration,
	Range,
	Melee,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterStateUpdated, ECharacterState, NewState);


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class PXII_API UPxiiCharSimulatorComponent : public UActorComponent
{
	GENERATED_BODY()



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UPxiiCharSimulatorComponent();

	UPROPERTY(BlueprintAssignable)
	FOnCharacterStateUpdated OnCharacterStateUpdated;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCharacterState(ECharacterState state);

	void SetCharacterState_Implementation(ECharacterState state);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetToExploration();

	void ResetToExploration_Implementation();

	UFUNCTION(BlueprintPure)
	ECharacterState GetCharacterState();

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	float ResetTimer = 5.0f;
	
	FTimerHandle StateHandler;

	ECharacterState CurrentState = ECharacterState::Exploration;
	
};
