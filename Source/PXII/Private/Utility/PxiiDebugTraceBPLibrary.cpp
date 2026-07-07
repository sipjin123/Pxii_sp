// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/PxiiDebugTraceBPLibrary.h"

#include "DrawDebugHelpers.h"


void UPxiiDebugTraceBPLibrary::DrawDebugSphereSimple(UObject* WorldContextObject, FVector Location, float Radius, FLinearColor Color, float Duration)
{
	if (!WorldContextObject) return;
	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return;
	DrawDebugSphere(World, Location, Radius, 12, Color.ToFColor(true), Duration < 0.0f, Duration);
}

void UPxiiDebugTraceBPLibrary::DrawDebugArrowSimple(const UObject* WorldContextObject, FVector Start, FVector End, FLinearColor Color, float Duration)
{
	if (!WorldContextObject) return;
	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return;
	DrawDebugDirectionalArrow(World, Start, End, 10.f, Color.ToFColor(true), false, Duration, 0, 1.f);
}