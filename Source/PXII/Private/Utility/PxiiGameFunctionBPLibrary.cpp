// Fill out your copyright notice in the Description page of Project Settings.

#include "Utility/PxiiGameFunctionBPLibrary.h"

#include "Abilities/Async/AbilityAsync_WaitGameplayTag.h"

void UPxiiGameFunctionBPLibrary::CountDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval,
                                           float& OutTimeRemaining, ECountDownActionInput CountDownInput, ECountDownActionOutput& CountDownOutput,
                                           FLatentActionInfo LatentInfo)
{
	UWorld* World = nullptr;
	
	if (GEngine)
	{
		World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}
	
	if (!World)
	{
		return;
	}
	
	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
	FPxiiCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FPxiiCountDownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);
	
	if (CountDownInput == ECountDownActionInput::Start)
	{
		if (!FoundAction)
		{
			LatentActionManager.AddNewAction(
				LatentInfo.CallbackTarget, 
				LatentInfo.UUID, 
				new FPxiiCountDownAction(TotalTime, UpdateInterval, OutTimeRemaining, CountDownOutput, LatentInfo));
		}
	}
	
	if (CountDownInput == ECountDownActionInput::Cancel)
	{
		if (FoundAction)
		{
			FoundAction->CancelAction();
		}
	}
}
