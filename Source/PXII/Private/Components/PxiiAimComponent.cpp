#include "Components/PxiiAimComponent.h"
#include "Engine/OverlapResult.h"
#include "Targeting/Targetable.h"
#include "Utility/PXIILogUtility.h"

UPxiiAimComponent::UPxiiAimComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.0f;
}

void UPxiiAimComponent::BeginPlay()
{
	Super::BeginPlay();
	ScanTimer = 0.0f;

	// AimTimeline = NewObject<UTimelineComponent>(this);
	// AimTimeline->AddInterpFloat(AimCurve,FOnTimelineFloatStatic::CreateUObject(this, &UPxiiAimComponent::UpdateAim));
	// AimTimeline->RegisterComponent();
	
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if(ASC)
	{
		FGameplayTag adsTag = FGameplayTag::RequestGameplayTag("Pxii.Combat.State.ADS");
		ASC->RegisterGameplayTagEvent( adsTag,	EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UPxiiAimComponent::OnAimTagChanged);
		PXII_LOG(ELogCategory::Controls, Log, TEXT("DZ_LOG:: Component BEGIN"));		
	}
}

void UPxiiAimComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(CurrentTarget.IsValid())
	{
		bool isLockable = CurrentTarget->GetClass()->ImplementsInterface(UTargetable::StaticClass()) && ITargetable::Execute_IsLockable(CurrentTarget.Get());
		if(!isLockable)
		{
			ClearTarget();
		}
	}
	
	ScanTimer -= DeltaTime;
	if(ScanTimer <= 0.0f)
	{
		ScanTimer = ScanInterval;
		ScanAndScoreTarget();
	}
}

AActor* UPxiiAimComponent::GetCurrentTarget() const
{
	return CurrentTarget.Get();
}

void UPxiiAimComponent::SetCurrentTarget(AActor* Target)
{
	AActor* oldTarget = CurrentTarget.Get();
	if(oldTarget == Target)
	{
		return;
	}

	CurrentTarget = Target;
	OnTargetUpdated.Broadcast(oldTarget, Target);
}

void UPxiiAimComponent::ScanAndScoreTarget()
{
	TArray<FTargetCandidate> targets = GetPotentialTargets();
	if(targets.Num() == 0)
	{
		if(CurrentTarget.IsValid())
		{
			ClearTarget();
		}
		return;
	}

	targets.Sort([](const FTargetCandidate& A, const FTargetCandidate& B)
	{
		return A.Score > B.Score;
	});

	const FTargetCandidate& topTarget = targets[0];
	if(topTarget.Target != CurrentTarget.Get())
	{
		SetCurrentTarget(topTarget.Target);
	}
}

float UPxiiAimComponent::ScoreCandidate(const AActor* Candidate, const FVector& ViewLocation, const FVector& ViewDirection) const
{
	FVector aimLocation = ITargetable::Execute_GetAimSocketLocation(Candidate);
	FVector toTarget = aimLocation - ViewLocation;
	float dist = toTarget.Size();
	FVector toTargetDir = toTarget.GetSafeNormal();

	const float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ViewDirection.GetSafeNormal(), toTargetDir)));

	float angleScore = 1.0f - FMath::Clamp(angle / MaxAngleDegrees, 0.0f, 1.0f);
	float distanceScore = 1.0f - FMath::Clamp(dist / MaxRange, 0.0f, 1.0f);
	float prioScore = 1.0f - FMath::Clamp(ITargetable::Execute_GetThreatPriority(Candidate), 0.0f, 1.0f);

	float score = (AngleWeight * angleScore) + (DistanceWeight * distanceScore) + (PriorityWeight * prioScore);

	if(CurrentTarget.IsValid() && CurrentTarget.Get() == Candidate)
	{
		score += StickyLockMargin;	
	}

	return score;
}

TArray<FTargetCandidate> UPxiiAimComponent::GetPotentialTargets() const
{
	TArray<FTargetCandidate> targets;

	FVector ViewLocation;
	FVector ViewDirection;
	if(!GetViewPoint(ViewLocation, ViewDirection))
	{
		return targets;
	}

	TArray<FOverlapResult> overlaps;
	FCollisionQueryParams params(SCENE_QUERY_STAT(TargetingScan), false, GetOwner());
	GetWorld()->OverlapMultiByChannel(overlaps, ViewLocation, FQuat::Identity, ECC_Pawn,
		FCollisionShape::MakeSphere(MaxRange), params);
	
	for(const FOverlapResult& hit : overlaps)
	{
		AActor* actor = hit.GetActor();
		if(!actor || actor == GetOwner())
		{
			continue;
		}
		if(!actor->GetClass()->ImplementsInterface(UTargetable::StaticClass()))
		{
			continue;
		}
		if(!ITargetable::Execute_IsLockable(actor))
		{
			continue;
		}

		const FVector aimLocation = ITargetable::Execute_GetAimSocketLocation(actor);
		const FVector toTarget = (aimLocation - ViewLocation).GetSafeNormal();
		const float angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ViewDirection.GetSafeNormal(), toTarget)));
		
		if(angle > MaxAngleDegrees)
		{
			continue;
		}

		if(RequireLineOfSight && !HasLineOfSight(actor, ViewLocation))
		{
			continue;
		}

		FTargetCandidate hitTarget;
		hitTarget.Target = actor;
		hitTarget.Score = ScoreCandidate(actor, ViewLocation, ViewDirection);
		targets.Add(hitTarget);
	}

	return targets;
}

bool UPxiiAimComponent::HasLineOfSight(const AActor* Candidate, const FVector& FromLoc) const
{
	FVector AimLoc = ITargetable::Execute_GetAimSocketLocation(Candidate);
	FCollisionQueryParams Params(SCENE_QUERY_STAT(TargetingLOS), false, GetOwner());
	Params.AddIgnoredActor(Candidate);

	FHitResult Hit;
	const bool blocked = GetWorld()->LineTraceSingleByChannel(Hit, FromLoc, AimLoc, ECC_Visibility, Params);
	return !blocked || Hit.GetActor() == Candidate;
}

bool UPxiiAimComponent::GetViewPoint(FVector& OutLoc, FVector& OutDir) const
{
	const APawn* owner = Cast<APawn>(GetOwner());
	if(!owner)
	{
		PXII_LOG(ELogCategory::Aim, Error, TEXT("Owner is null"));
		return false;
	}

	const APlayerController* PC = Cast<APlayerController>(owner->GetController());
	if(!PC || !PC->PlayerCameraManager)
	{
		PXII_LOG(ELogCategory::Aim, Error, TEXT("Camera Manager is null"));
		return false;
	}

	OutLoc = PC->PlayerCameraManager->GetCameraLocation();
	OutDir = PC->PlayerCameraManager->GetCameraRotation().Vector();
	return true;
}

bool UPxiiAimComponent::HasTarget() const
{
	return CurrentTarget.IsValid();
}

void UPxiiAimComponent::ClearTarget()
{
	SetCurrentTarget(nullptr);
}

void UPxiiAimComponent::StartAim_Implementation()
{
	// AimTimeline->PlayFromStart();
}

void UPxiiAimComponent::StopAim_Implementation()
{
	// AimTimeline->Reverse();
}

void UPxiiAimComponent::OnAimTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		StartAim();
	}
	else
	{
		StopAim();
	}
}
