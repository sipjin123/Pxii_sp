// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/GameplayTimeDilationSubsystem.h"
#include "Containers/Map.h"
#include "Enum/TimescaleChannel.h"

void UGameplayTimeDilationSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
	
	Channels.Add(ETimescaleChannel::Global, FTimeChannelData());
	Channels.Add(ETimescaleChannel::Player, FTimeChannelData());
	Channels.Add(ETimescaleChannel::Enemy, FTimeChannelData());
	Channels.Add(ETimescaleChannel::Projectile, FTimeChannelData());
}

void UGameplayTimeDilationSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TStatId UGameplayTimeDilationSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UGameplayTimeDilationSubsystem, STATGROUP_Tickables);
	//return Super::GetStatId();
}

void UGameplayTimeDilationSubsystem::SetTimeScale(ETimescaleChannel Channel, float TimeScale)
{
	if (FTimeChannelData* Data = Channels.Find(Channel))
	{
		Data->CurrentScale = TimeScale;
	}
}

float UGameplayTimeDilationSubsystem::GetTimeScale(ETimescaleChannel Channel) const
{
	if (const FTimeChannelData* Data = Channels.Find(Channel))
	{
		return Data->CurrentScale;
	}

	return 1.f;
}