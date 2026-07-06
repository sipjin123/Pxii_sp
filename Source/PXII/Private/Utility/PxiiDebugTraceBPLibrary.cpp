// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/PxiiDebugTraceBPLibrary.h"

#include "DrawDebugHelpers.h"


void UPxiiDebugTraceBPLibrary::DrawDebugSphereSimple(UObject* WorldContextObject, FVector Location, float Radius,
                                                     FLinearColor Color, float Duration)
{
	if (!WorldContextObject) return;

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return;

	DrawDebugSphere(
		World,
		Location,
		Radius,
		12, // Segments (reasonable default)
		Color.ToFColor(true),
		Duration < 0.0f, // persistent lines if < 0
		Duration
	);
}
