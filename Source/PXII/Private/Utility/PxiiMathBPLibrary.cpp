// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/PxiiMathBPLibrary.h"

FVector UPxiiMathBPLibrary::GetLocationInFrontOfActor(const AActor* Actor, float Distance)
{
	if (!Actor) return FVector::ZeroVector;
	return Actor->GetActorLocation() + Actor->GetActorForwardVector() * Distance;
}
