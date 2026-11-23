// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PxiiCountDownAction.generated.h"

UENUM()
enum class ECountDownActionInput : uint8
{ 
	Start,
	Cancel,
};

UENUM()
enum class ECountDownActionOutput : uint8
{
	Updated,
	Completed,
	Cancelled,
};
/**
 * 
 */
class FPxiiCountDownAction : public FPendingLatentAction
{
public:
	FPxiiCountDownAction(float InTotalCountDownTime, float InUpdateInterval, float& OutTimeRemaining,
		ECountDownActionOutput& OutActionOutput, const FLatentActionInfo& LatentInfo) :
		bNeedToCancel(false),
		TotalCountDownTime(InTotalCountDownTime),
		UpdateInterval(InUpdateInterval),
		TimeRemaining(OutTimeRemaining),
		CountDownOutput(OutActionOutput),
		ExecutionFunction(LatentInfo.ExecutionFunction),
		OutputLink(LatentInfo.Linkage),
		CallbackTarget(LatentInfo.CallbackTarget),
		ElapsedInterval(0.f),
		ElapsedTimeSinceStart(0.f){};
	
	virtual void UpdateOperation(FLatentResponse& Response) override;
	
	void CancelAction();

private:
	bool bNeedToCancel;
	float TotalCountDownTime;
	float UpdateInterval;
	float& TimeRemaining;
	ECountDownActionOutput& CountDownOutput;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	float ElapsedInterval;
	float ElapsedTimeSinceStart;
};
