#pragma once
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "Character/PxiiCharacter.h"
#include "PxiiAimComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTargetUpdated, AActor*, OldTarget, AActor*, NewTarget);

USTRUCT(BlueprintType)
struct FTargetCandidate
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	AActor* Target = nullptr;

	UPROPERTY(BlueprintReadOnly)
	float Score = 0.0f;
};

USTRUCT(BlueprintType)
struct FHitInformation
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FHitResult HitResult = FHitResult();

	UPROPERTY(BlueprintReadOnly)
	FVector TraceStart = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly)
	FVector TraceEnd = FVector::ZeroVector;
	
	FHitInformation() = default;
	
	FHitInformation(const FHitResult& InHitResult, const FVector& InTraceStart, const FVector& InTraceEnd)
		: HitResult(InHitResult), TraceStart(InTraceStart), TraceEnd(InTraceEnd)
	{
		
	}
};


UCLASS(Blueprintable)
class PXII_API UPxiiAimComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UPxiiAimComponent();

	UPROPERTY(BlueprintAssignable)
	FOnTargetUpdated OnTargetUpdated;
	
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	AActor* GetCurrentTarget() const;

	UFUNCTION(BlueprintPure)
	bool HasTarget() const;

	UFUNCTION(BlueprintCallable)
	void ClearTarget();
	
	UFUNCTION(BlueprintNativeEvent)
	void StartAim();

	UFUNCTION(BlueprintNativeEvent)
	void StopAim();

	UFUNCTION()
	void OnAimTagChanged(const FGameplayTag Tag, int32 NewCount);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAim(float DeltaTime);

	UFUNCTION(BlueprintPure)
	bool GetIsADSActive();

protected:

	UPROPERTY()
	bool AimActive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxRange = 3000.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxAngleDegrees = 35.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ScanInterval = 0.12f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AngleWeight = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DistanceWeight = 0.3f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PriorityWeight = 0.2f;;

	float StickyLockMargin = 0.15f;
	bool RequireLineOfSight = true;
	
	
private:

	TWeakObjectPtr<AActor> CurrentTarget;

	bool bIsAiming;
	float ScanTimer = 0.0f;

	void SetCurrentTarget(AActor* Target);

	void ScanAndScoreTarget();

	float ScoreCandidate(const AActor* Candidate, const FVector& ViewLocation, const FVector& ViewDirection) const;

	TArray<FTargetCandidate> GetPotentialTargets() const;

	bool HasLineOfSight(const AActor* Candidate, const FVector& FromLoc) const;

	bool GetViewPoint(FVector& OutLoc, FVector& OutDir) const;
};
