#pragma once
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayTag.h"
#include "Character/PxiiCharacter.h"
#include "Components/TimelineComponent.h"
#include "PxiiAimComponent.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiAimComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void StartAim();

	UFUNCTION(BlueprintNativeEvent)
	void StopAim();

	UFUNCTION()
	void OnAimTagChanged(const FGameplayTag Tag, int32 NewCount);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAim(float DeltaTime);

	FVector GetAimPoint() const;
	AActor* GetCurrentTarget() const;

protected:
	
	UPROPERTY()
	UTimelineComponent* AimTimeline;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> AimCurve;
	
private:
	
	bool bIsAiming;
};
