// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ProjectileSubsystem.h"

DEFINE_LOG_CATEGORY(LogProjectileSubsystem);
void UProjectileSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
}

void UProjectileSubsystem::TestCppFunc()
{
	UE_LOG(LogProjectileSubsystem,Warning,TEXT("Test Success"));
}
