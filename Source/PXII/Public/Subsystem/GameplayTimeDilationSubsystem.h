// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTimeDilationSubsystem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTimeChannelData
{
	GENERATED_BODY()
	float CurrentScale = 1.f;
	
	float TargetScale = 1.f;
	float BlendSpeed = 8.f;
	float CachedScale = 1.f;
};

USTRUCT(BlueprintType)
struct FTimeModifier
{
	GENERATED_BODY()
	FGuid Id;
	ETimescaleChannel Channel;
	FGameplayTag Source;
	float Scale = 1.f;
};

UCLASS(Abstract, Blueprintable)
class PXII_API UGameplayTimeDilationSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
public:
	UFUNCTION(BlueprintCallable)
	void SetTimeScale(ETimescaleChannel Channel, float TimeScale);
	UFUNCTION(BlueprintCallable)
	float GetTimeScale(ETimescaleChannel Channel) const;
	
private:
	UPROPERTY()
	TMap<ETimescaleChannel, FTimeChannelData> Channels;
};
