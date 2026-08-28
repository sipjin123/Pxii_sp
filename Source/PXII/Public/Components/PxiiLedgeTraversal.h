#pragma once
#include "Character/PxiiCharacter.h"
#include "Components/ActorComponent.h"
#include "MoverComponent.h"
#include "MovementMode/LedgeMovementMode.h"
#include "PxiiLedgeTraversal.generated.h"

class ACharacter;
class UCharacterMovementComponent;
class UAnimMontage;
class UMotionWarpingComponent;
class UCapsuleComponent;

UENUM(BlueprintType)
enum class ELedgeHangState : uint8
{
	None,
	Grabbing,      // brief transition animation into the hang
	Hanging,       // idle, locked to ledge
	Shimmying,     // actively moving along the ledge
	TurningCorner, // navigating an inside/outside corner
	ClimbingUp,    // exiting via mantle
	DroppingOff    // exiting via release/fall
	,
	Jumping
};

USTRUCT(BlueprintType)
struct FLedgeHangInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bValid = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector EdgeLocation = FVector::ZeroVector;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector WallNormal = FVector::ZeroVector;

	// points away from wall, toward player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector LedgeDirection = FVector::ZeroVector;

	// direction the ledge edge runs (for shimmy)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ObstacleHeight = 0.0f;
};

UCLASS(Blueprintable)
class PXII_API UPxiiLedgeTraversal : public UActorComponent
{
	GENERATED_BODY()
	
public:
	
    UPxiiLedgeTraversal();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "LedgeHang")
	void UpdateMovementMode(FName movementMode);

	void UpdateMovementMode_Implementation(FName movementMode);

	UFUNCTION(BlueprintCallable, Category = "LedgeHang")
	bool TryGrabLedge();
	
	UFUNCTION(BlueprintCallable, Category = "LedgeHang")
	bool TryLedgeJump(FVector2D JumpInput);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayLedgeEntry(UAnimMontage* montage, FLedgeHangInfo ledgeInfo);
	void PlayLedgeEntry_Implementation(UAnimMontage* montage, FLedgeHangInfo ledgeInfo);

	UFUNCTION(BlueprintCallable)
	void PostLedgeEntry(FLedgeHangInfo ledgeInfo);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayShimmyMontage(UAnimMontage* montage, FLedgeHangInfo ledgeInfo);
	void PlayShimmyMontage_Implementation(UAnimMontage* montage, FLedgeHangInfo ledgeInfo);
	
	UFUNCTION(BlueprintCallable)
	void PostShimmy(FLedgeHangInfo ledgeInfo);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayLedgeJump(UAnimMontage* montage, FLedgeHangInfo ledgeInfo);
	void PlayLedgeJump_Implementation(UAnimMontage* montage, FLedgeHangInfo ledgeInfo);

	UFUNCTION(BlueprintCallable)
	void PostLedgeJump(FLedgeHangInfo ledgeInfo);
	
    UFUNCTION(BlueprintCallable, Category = "LedgeHang")
    void SetShimmyInput(float InputAxis); // -1 to 1, from player input

    UFUNCTION(BlueprintCallable, Category = "LedgeHang")
    void TryClimbUp();

    UFUNCTION(BlueprintCallable, Category = "LedgeHang")
    void DropOff();

    UFUNCTION(BlueprintPure, Category = "LedgeHang")
    ELedgeHangState GetState() const; 

	UFUNCTION(BlueprintPure)
	FVector GetWallLocation();
	
	UFUNCTION(BlueprintPure)
	FVector GetLedgeLocation() const;

	UFUNCTION(BlueprintPure)
	FVector GetAdjustedLedgeLocation(FLedgeHangInfo ledgeInfo) const;

	UFUNCTION(BlueprintPure)
	FLedgeHangInfo GetCurrentLedge() const;

protected:
	
    virtual void BeginPlay() override;

    // --- Grab detection ---
    UPROPERTY(EditDefaultsOnly, Category = "Ledge Traversal|Detection")
    float GrabTraceDistance = 90.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Ledge Traversal|Detection", meta = (AllowPrivateAccess = "true"))
    float MinGrabHeight = 100.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Ledge Traversal|Detection" , meta = (AllowPrivateAccess = "true"))
    float MaxGrabHeight = 400.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Ledge Traversal|Detection")
    float HangHandOffsetZ = -20.0f; // hands slightly below traced edge for visual grip

	UPROPERTY(EditAnywhere, Category = "Ledge Traversal|Jump")
	float MinLedgeJumpDistance = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Ledge Traversal|Jump")
	float MaxLedgeJumpDistance = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Ledge Traversal|Jump")
	float LedgeJumpMaxHeightDelta = 80.0f;

	// How far out along the jump direction to launch the trace from (clears the current wall)
	UPROPERTY(EditAnywhere, Category = "Ledge Traversal|Jump")
	float LedgeJumpLaunchOffset = 40.0f;
	
	UPROPERTY(EditAnywhere, Category = "Ledge Traversal|Jump")
	float LedgeJumpArcHeight = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Ledge Traversal|Jump")
	float MinHorizontalLedgeJumpDistance = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Ledge Traversal|Jump")
	float MaxHorizontalLedgeJumpDistance = 400.0f;
	
    // --- Shimmy ---
    UPROPERTY(EditDefaultsOnly, Category = "Ledge Traversal|Shimmy")
    float ShimmySpeed = 130.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Ledge Traversal|Shimmy")
    float ShimmyProbeAheadDistance = 40.0f; // how far ahead along the ledge to check validity

    UPROPERTY(EditDefaultsOnly, Category = "Ledge Traversal|Shimmy")
    float CornerDetectionAngleThreshold = 30.0f; // wall normal change beyond this = corner

    // --- Animation ---
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ledge Traversal|Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* GrabMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ledge Traversal|Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* HangIdleMontage; // looping

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ledge Traversal|Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* ShimmyLeftMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ledge Traversal|Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* ShimmyRightMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ledge Traversal|Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* ClimbUpMontage; 

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ledge Traversal|Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* InsideCornerMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ledge Traversal|Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* OutsideCornerMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ledge Traversal|Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LedgeJumpMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ledge Traversal|Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LedgeDownMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ledge Traversal|Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LedgeRightJumpMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ledge Traversal|Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LedgeLeftMontage;

private:
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    APxiiCharacter* OwnerCharacter;

    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UMoverComponent> MoverComp;



	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UMotionWarpingComponent> MotionWarpingComp;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector CurrentHangLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DebugDuration = 10.0f;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName MotionWarpKey = FName("HangLocation");

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CapsuleHalfHeight;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CapsuleRadius;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimInstance> AnimInst;
	
	UPROPERTY()
	TObjectPtr<UCapsuleComponent> CapsuleComp;
	
	UPROPERTY()
	bool CanShimmy = true;

    ELedgeHangState CurrentState = ELedgeHangState::None;
    FLedgeHangInfo CurrentLedge;
    float CurrentShimmyInput = 0.0f;
	
	
    FLedgeHangInfo TraceForLedge(const FVector& FromLocation, const FVector& Forward) const;
    FLedgeHangInfo TraceForLedgeAtOffset(float LateralOffset) const;
	FLedgeHangInfo TraceForLedgeJump(const FVector2D& JumpInput) const;
	
    void UpdateShimmy(float DeltaTime);
	void UpdateMotionWarp(const FLedgeHangInfo& LedgeInfo);
    void HandleCornerTransition(const FLedgeHangInfo& NewLedgeInfo, bool bIsInsideCorner);
    void SnapToLedge(const FLedgeHangInfo& LedgeInfo);

	UAnimInstance* GetAnimInstance();

    UFUNCTION()
    void OnClimbMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
