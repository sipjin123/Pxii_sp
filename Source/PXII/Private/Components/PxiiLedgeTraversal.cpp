#include "Components/PxiiLedgeTraversal.h"

#include "DrawDebugHelpers.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "MoverComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionWarpingComponent.h"
#include "Animation/AnimInstance.h"
#include "Character/PxiiCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Utility/PXIILogUtility.h"

UPxiiLedgeTraversal::UPxiiLedgeTraversal()
{
    PrimaryComponentTick.bCanEverTick = true;
}

// void UPxiiLedgeTraversal::BeginPlay()
// {
//     Super::BeginPlay();
//     
//     OwnerCharacter = Cast<APxiiCharacter>(GetOwner());
//     
//     if (OwnerCharacter)
//     {
//         CapsuleComp = OwnerCharacter->GetComponentByClass<UCapsuleComponent>();
//         CapsuleHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
//         CapsuleRadius = CapsuleComp->GetScaledCapsuleRadius();
//         MoverComp = OwnerCharacter->GetComponentByClass<UMoverComponent>();
//         MotionWarpingComp = OwnerCharacter->FindComponentByClass<UMotionWarpingComponent>();
//
//     }
// }
//
// FLedgeHangInfo UPxiiLedgeTraversal::TraceForLedge(const FVector& FromLocation, const FVector& Forward) const
// {
//     FLedgeHangInfo Info;
//     if (!OwnerCharacter)
//     {
//         return Info;   
//     }
//
//     FCollisionQueryParams Params;
//     Params.AddIgnoredActor(OwnerCharacter);
//
//     // Forward trace to find wall
//     FVector TraceEnd = FromLocation + (Forward * GrabTraceDistance);
//     FHitResult WallHit;
//     if (!GetWorld()->LineTraceSingleByChannel(WallHit, FromLocation, TraceEnd, ECC_Visibility, Params))
//     {
//         DrawDebugSphere(GetWorld(), FromLocation, 5.0f, 8, FColor::Cyan, false,DebugDuration);
//         DrawDebugLine(GetWorld(), FromLocation, TraceEnd, FColor::Red, false,DebugDuration, 0, 2.0f);
//         return Info;
//     }
//     
//     DrawDebugLine(GetWorld(), FromLocation, TraceEnd, WallHit.bBlockingHit ? FColor::Red : FColor::Green, false,
// DebugDuration, 0, 2.0f);
//
//     if (WallHit.bBlockingHit)
//     {
//         DrawDebugSphere(GetWorld(), WallHit.ImpactPoint, 5.0f, 8, FColor::Yellow, false,DebugDuration);
//     }
//     
//     // Downward trace from above the wall hit to find the actual edge
//     FVector ProbeStart = WallHit.ImpactPoint + (Forward * -5.0f) + FVector(0, 0, MaxGrabHeight * 0.5f);
//     FVector ProbeEnd = ProbeStart - FVector(0, 0, MaxGrabHeight);
//
//     FHitResult EdgeHit;
//     
//     if (!GetWorld()->LineTraceSingleByChannel(EdgeHit, ProbeStart, ProbeEnd, ECC_Visibility, Params))
//     {
//         return Info;
//     }
//
//     DrawDebugLine(GetWorld(), ProbeStart, ProbeEnd, EdgeHit.bBlockingHit ? FColor::Red : FColor::Green, false,
//     DebugDuration, 0, 2.0f);
//
//     if (EdgeHit.bBlockingHit)
//     {
//         DrawDebugSphere(GetWorld(), EdgeHit.ImpactPoint, 5.0f, 8, FColor::Yellow, false,DebugDuration);
//     }
//
//     float Height = EdgeHit.ImpactPoint.Z - OwnerCharacter->GetActorLocation().Z;
//     PXII_LOG(ELogCategory::General, Log, TEXT("DZ_LOG: ImpactPoint: %s  OwnerLocation: %s Height: %f"), *EdgeHit.ImpactPoint.ToString(), *OwnerCharacter->GetActorLocation().ToString(), Height );
//     if (Height < MinGrabHeight || Height > MaxGrabHeight)
//     {
//         return Info;
//     }
//
//     // Ledge direction — perpendicular to the wall normal, on the horizontal plane
//     Info.WallNormal = WallHit.ImpactNormal;
//     Info.LedgeDirection = FVector::CrossProduct(FVector::UpVector, WallHit.ImpactNormal).GetSafeNormal();
//     Info.EdgeLocation = EdgeHit.ImpactPoint;
//     Info.ObstacleHeight = Height;
//     Info.bValid = true;
//
//     return Info;
// }
//
// void UPxiiLedgeTraversal::UpdateMovementMode_Implementation(FName movementMode)
// {
//     
// }
//
// bool UPxiiLedgeTraversal::TryGrabLedge()
// {
//     if (CurrentState != ELedgeHangState::None || !OwnerCharacter)
//     {
//         return false;   
//     }
//
//     FLedgeHangInfo LedgeInfo = TraceForLedge(OwnerCharacter->GetActorLocation() + FVector(0, 0, 40.0f),
//         OwnerCharacter->GetActorForwardVector());
//
//     if (!LedgeInfo.bValid)
//     {
//         return false;  
//     } 
//
//     CurrentState = ELedgeHangState::Grabbing;
//
//     if (GrabMontage)
//     {
//         PlayLedgeEntry(GrabMontage, LedgeInfo);
//     }
//
//     return true;
// }
//
// void UPxiiLedgeTraversal::UpdateShimmy(float DeltaTime)
// {
//     if (FMath::IsNearlyZero(CurrentShimmyInput, 0.1f))
//     {
//         if (CurrentState == ELedgeHangState::Shimmying)
//         {
//             CurrentState = ELedgeHangState::Hanging;
//         }
//         return;
//     }
//
//     if(!CanShimmy)
//     {
//         return;
//     }
//     if(GetAnimInstance()->IsAnyMontagePlaying())
//     {
//         return;
//     }
//     
//     CanShimmy = false;
//     CurrentState = ELedgeHangState::Shimmying;
//
//     float MoveDir = FMath::Sign(CurrentShimmyInput);
//     float ProbeDistance = ShimmyProbeAheadDistance * MoveDir;
//
//     const FLedgeHangInfo AheadLedge = TraceForLedgeAtOffset(ProbeDistance);
//     PXII_LOG(ELogCategory::LedgeTraversal, Log, TEXT("CAN SHIMY"));
//     
//     if (!AheadLedge.bValid)
//     {
//         PXII_LOG(ELogCategory::LedgeTraversal, Log, TEXT("Set SHIMY to true"));
//         CanShimmy = true;
//         return;
//     }
//
//     const float Dot = FMath::Clamp(FVector::DotProduct(CurrentLedge.WallNormal, AheadLedge.WallNormal), -1.0f, 1.0f);
//     float NormalAngleDelta = FMath::RadiansToDegrees(FMath::Acos(Dot));
//
//     if (NormalAngleDelta > CornerDetectionAngleThreshold)
//     {
//         // Determine inside vs outside corner via cross product sign
//         FVector Cross = FVector::CrossProduct(CurrentLedge.WallNormal, AheadLedge.WallNormal);
//         bool bIsInsideCorner = FVector::DotProduct(Cross, FVector::UpVector) * MoveDir > 0.0f;
//
//         HandleCornerTransition(AheadLedge, bIsInsideCorner);
//         CanShimmy = true;
//         PXII_LOG(ELogCategory::LedgeTraversal, Log, TEXT("Set SHIMY to true"));
//         return;
//     }
//     
//     // Play directional shimmy animation
//     UAnimMontage* DirMontage = (MoveDir > 0) ? ShimmyLeftMontage : ShimmyRightMontage;
//     if (GetAnimInstance() && DirMontage)
//     {
//         PlayShimmyMontage(DirMontage, AheadLedge);
//     }
//     else
//     {
//         PXII_LOG(ELogCategory::LedgeTraversal, Log, TEXT("Set SHIMY to true"));
//         CanShimmy = true;
//     }
// }
//
// bool UPxiiLedgeTraversal::TryLedgeJump(FVector2D JumpInput)
// {
//     if ((CurrentState != ELedgeHangState::Hanging && CurrentState != ELedgeHangState::Shimmying) || !OwnerCharacter)
//     {
//         return false;
//     }
//
//     const FLedgeHangInfo TargetLedge = TraceForLedgeJump(JumpInput);
//     if (!TargetLedge.bValid)
//     {
//         return false;
//     }
//
//     CurrentState = ELedgeHangState::Jumping;
//
//     UAnimMontage* DirMontage = (JumpInput.Y > 0) ? LedgeJumpMontage : LedgeDownMontage;
//     if(!FMath::IsNearlyZero(JumpInput.X))
//     {
//         DirMontage = (JumpInput.X > 0) ? LedgeRightJumpMontage : LedgeLeftMontage;
//     }
//     if (GetAnimInstance() && DirMontage)
//     {
//         PlayLedgeJump(DirMontage, TargetLedge);
//     }
//     else
//     {
//         // No montage authored — snap directly, same fallback pattern as HandleCornerTransition
//         CurrentLedge = TargetLedge;
//         SnapToLedge(CurrentLedge);
//         CurrentState = ELedgeHangState::Hanging;
//     }
//
//     return true;
//     
// }
//
// void UPxiiLedgeTraversal::PlayLedgeEntry_Implementation(UAnimMontage* montage, FLedgeHangInfo ledgeInfo)
// {
//     CurrentLedge = ledgeInfo;
//     UpdateMotionWarp(CurrentLedge);
//     CurrentHangLocation = GetAdjustedLedgeLocation(ledgeInfo);
// }
//
// void UPxiiLedgeTraversal::PostLedgeEntry(FLedgeHangInfo ledgeInfo)
// {
//     SnapToLedge(ledgeInfo);
//     CurrentState = ELedgeHangState::Hanging;
// }
//
// void UPxiiLedgeTraversal::PlayShimmyMontage_Implementation(UAnimMontage* montage, FLedgeHangInfo ledgeInfo)
// {
//     UpdateMotionWarp(ledgeInfo);
//
//     CurrentLedge = ledgeInfo;
//     CurrentHangLocation = GetAdjustedLedgeLocation(ledgeInfo);
// }
//
// void UPxiiLedgeTraversal::PostShimmy(FLedgeHangInfo ledgeInfo)
// {
//     CanShimmy = true;
//     SnapToLedge(CurrentLedge);
//     CurrentState = ELedgeHangState::Hanging;
//     PXII_LOG(ELogCategory::LedgeTraversal, Log, TEXT("Post SHIMY"));
// }
//
// void UPxiiLedgeTraversal::PlayLedgeJump_Implementation(UAnimMontage* montage, FLedgeHangInfo ledgeInfo)
// {
//     UpdateMotionWarp(ledgeInfo);
//     CurrentLedge = ledgeInfo;
//     CurrentHangLocation = GetAdjustedLedgeLocation(ledgeInfo);
// }
//
// void UPxiiLedgeTraversal::PostLedgeJump(FLedgeHangInfo ledgeInfo)
// {
//     SnapToLedge(ledgeInfo);
//     CurrentState = ELedgeHangState::Hanging;
// }
//
// void UPxiiLedgeTraversal::SnapToLedge(const FLedgeHangInfo& LedgeInfo)
// {
//     CapsuleHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
//     CapsuleRadius = CapsuleComp->GetScaledCapsuleRadius();
//
//     FVector HangLocation = GetAdjustedLedgeLocation(LedgeInfo);
//
//     UE_LOG(LogTemp, Log, TEXT("DZ_LOG:: Snap To Ledge: %s Edge Loc: %s"), *HangLocation.ToString(), *LedgeInfo.EdgeLocation.ToString());
//     DrawDebugSphere(GetWorld(), HangLocation, 25.0f, 8, FColor::Yellow, false,DebugDuration);
//     
//     FRotator HangRotation = (-LedgeInfo.WallNormal).Rotation();
//     HangRotation.Pitch = 0.0f;
//     HangRotation.Roll = 0.0f;
//     
//     OwnerCharacter->SetActorRotation(HangRotation);
//
//     DrawDebugSphere(GetWorld(), HangLocation, 25.0f, 8, FColor::Yellow, false,DebugDuration);
//
// }
//
// UAnimInstance* UPxiiLedgeTraversal::GetAnimInstance()
// {
//     if(!AnimInst)
//     {
//         AnimInst = OwnerCharacter->GetMesh()->GetAnimInstance();
//     }
//
//     return AnimInst;
// }
//
// void UPxiiLedgeTraversal::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
//     Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//     if (CurrentState == ELedgeHangState::Hanging || CurrentState == ELedgeHangState::Shimmying)
//     {
//         UpdateShimmy(DeltaTime);
//     }
// }
//
// FLedgeHangInfo UPxiiLedgeTraversal::TraceForLedgeAtOffset(float LateralOffset) const
// {
//     FVector ProbeOrigin = CurrentHangLocation
//         + (CurrentLedge.LedgeDirection * LateralOffset)
//         + FVector(0, 0, 40.0f);
//
//     return TraceForLedge(ProbeOrigin, -CurrentLedge.WallNormal * 1.0f); // trace toward the wall
// }
//
// FLedgeHangInfo UPxiiLedgeTraversal::TraceForLedgeJump(const FVector2D& JumpInput) const
// {
//     FLedgeHangInfo Info;
//     if (!OwnerCharacter || CurrentState == ELedgeHangState::None)
//     {
//         return Info;   
//     }
//     
//     // Build a world-space jump direction from lateral (along ledge) and outward (away from wall) input
//     FVector JumpDirection = (CurrentLedge.LedgeDirection * JumpInput.X) + (FVector::UpVector * JumpInput.Y);
//     if (JumpDirection.IsNearlyZero())
//     {
//         JumpDirection = FVector::UpVector;
//     }
//
//     JumpDirection = JumpDirection.GetSafeNormal();
//
//     FCollisionQueryParams Params;
//     Params.AddIgnoredActor(OwnerCharacter);
//
//     FVector LaunchOrigin = CurrentHangLocation + (FVector::ForwardVector * 5.f);
//     FVector TraceEnd = LaunchOrigin + (JumpDirection * MaxLedgeJumpDistance);
//
//     if(!FMath::IsNearlyZero(JumpInput.X))
//     {
//         LaunchOrigin = CurrentHangLocation + (FVector::ForwardVector * 5.f) + (FVector::RightVector * JumpInput.X * MaxHorizontalLedgeJumpDistance);
//         TraceEnd = LaunchOrigin + (FVector::UpVector * MaxLedgeJumpDistance);
//     }
//
//     FHitResult WallHit;
//     if (!GetWorld()->LineTraceSingleByChannel(WallHit, LaunchOrigin, TraceEnd, ECC_Visibility, Params))
//     {
//         DrawDebugSphere(GetWorld(), LaunchOrigin, 5.0f, 8, FColor::Blue, false, DebugDuration);
//         DrawDebugLine(GetWorld(), LaunchOrigin, TraceEnd, FColor::Orange, false,
//     DebugDuration, 0, 2.0f);
//         return Info;
//     }
//
//     DrawDebugLine(GetWorld(), LaunchOrigin, TraceEnd, WallHit.bBlockingHit ? FColor::Red : FColor::Green, false,
//         DebugDuration, 0, 2.0f);
//
//     if (!WallHit.bBlockingHit)
//     {
//         return Info;
//     }
//
//     DrawDebugSphere(GetWorld(), WallHit.ImpactPoint, 5.0f, 8, FColor::Blue, false, DebugDuration);
//
//     // Reject the wall we're already hanging on, so a near-zero input doesn't just re-grab in place
//     if (FVector::DotProduct(WallHit.ImpactNormal, CurrentLedge.WallNormal) > 0.9f
//         && FVector::DistSquared(WallHit.ImpactPoint, CurrentLedge.EdgeLocation) < FMath::Square(MinLedgeJumpDistance))
//     {
//         return Info;
//     }
//
//     // Downward probe from above the wall hit to find the actual edge, same approach as TraceForLedge
//     FVector jumpDir = FVector::UpVector * JumpInput.Y;
//     FVector ImpactPointOffset = WallHit.ImpactPoint;
//     FVector ProbeStart = ImpactPointOffset + (jumpDir * 5.0f) + (jumpDir * MaxLedgeJumpDistance);
//     FVector ProbeEnd = ProbeStart - jumpDir * MaxLedgeJumpDistance;
//     
//     if(!FMath::IsNearlyZero(JumpInput.X))
//     {
//         jumpDir = FVector::DownVector * JumpInput.X;
//         ImpactPointOffset = WallHit.ImpactPoint + FVector::UpVector * 100.0f;
//         ProbeStart = ImpactPointOffset + (jumpDir * 5.0f);
//         ProbeEnd = ProbeStart + jumpDir * MaxLedgeJumpDistance;    
//     }
//
//     if(!FMath::IsNearlyZero(JumpInput.Y))
//     {
//         jumpDir = FVector::UpVector * JumpInput.Y;
//
//         ProbeStart = ImpactPointOffset + (jumpDir * 5.0f) + (jumpDir * MaxLedgeJumpDistance);
//         ProbeEnd = ProbeStart - jumpDir * MaxLedgeJumpDistance;
//
//         if(JumpInput.Y < 0)
//         {
//             // ProbeStart = GetLedgeLocation() + (jumpDir * 5.0f) + (jumpDir * 60.0f);
//             ProbeStart = ImpactPointOffset + (jumpDir * 5.0f) + (jumpDir * 60.0f);
//             ProbeEnd = ProbeStart + jumpDir * MaxLedgeJumpDistance;
//         }
//     }
//
//     
//     DrawDebugSphere(GetWorld(), ProbeStart, 10.0f, 8, FColor::Yellow, false, DebugDuration);
//     DrawDebugLine(GetWorld(), ProbeStart, ProbeEnd, FColor::Orange, false, DebugDuration, 0, 2.0f);
//
//     FHitResult EdgeHit;
//     if (!GetWorld()->LineTraceSingleByChannel(EdgeHit, ProbeStart, ProbeEnd, ECC_Visibility, Params))
//     {
//         return Info;
//     }
//     
//     if (!EdgeHit.bBlockingHit)
//     {
//         return Info;
//     }
//
//     DrawDebugSphere(GetWorld(), EdgeHit.ImpactPoint, 10.0f, 8, FColor::Green, false, DebugDuration);
//
//     // Height delta relative to the ledge we're leaving, not the character root — we're mid-air, not standing
//     const float HeightDelta = FMath::Abs(EdgeHit.ImpactPoint.Z - CurrentLedge.EdgeLocation.Z);
//     if (HeightDelta > LedgeJumpMaxHeightDelta)
//     {
//         return Info;
//     }
//
//     const float JumpDistance = FVector::Dist(CurrentLedge.EdgeLocation, EdgeHit.ImpactPoint);
//     if (JumpDistance < MinLedgeJumpDistance || JumpDistance > MaxLedgeJumpDistance)
//     {
//         return Info;
//     }
//
//     Info.WallNormal = WallHit.ImpactNormal;
//     Info.LedgeDirection = FVector::CrossProduct(FVector::RightVector, WallHit.ImpactNormal).GetSafeNormal();
//     Info.EdgeLocation = EdgeHit.ImpactPoint;
//     Info.ObstacleHeight = EdgeHit.ImpactPoint.Z - OwnerCharacter->GetActorLocation().Z;
//     Info.bValid = true;
//
//     return Info;
// }
//
// void UPxiiLedgeTraversal::UpdateMotionWarp(const FLedgeHangInfo& LedgeInfo)
// {
//     if (MotionWarpingComp)
//     {
//         FVector TopSurface = LedgeInfo.EdgeLocation + (LedgeInfo.WallNormal * CapsuleRadius) + FVector(0, 0, HangHandOffsetZ - CapsuleHalfHeight);
//         MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(MotionWarpKey, TopSurface);
//         DrawDebugSphere(GetWorld(), TopSurface, 5.0f, 8, FColor::Purple, false,DebugDuration);
//     }
// }
//
// void UPxiiLedgeTraversal::SetShimmyInput(float InputAxis)
// {
//     CurrentShimmyInput = InputAxis;
// }
//
// void UPxiiLedgeTraversal::HandleCornerTransition(const FLedgeHangInfo& NewLedgeInfo, bool bIsInsideCorner)
// {
//     CurrentState = ELedgeHangState::TurningCorner;
//
//     UAnimMontage* CornerMontage = bIsInsideCorner ? InsideCornerMontage : OutsideCornerMontage;
//
//     if (MotionWarpingComp)
//     {
//         MotionWarpingComp->AddOrUpdateWarpTargetFromLocationAndRotation(
//             FName("CornerTarget"), NewLedgeInfo.EdgeLocation, (-NewLedgeInfo.WallNormal).Rotation());
//     }
//
//     if (GetAnimInstance() && CornerMontage)
//     {
//         GetAnimInstance()->Montage_Play(CornerMontage);
//
//         FOnMontageEnded EndDelegate;
//         EndDelegate.BindLambda([this, NewLedgeInfo](UAnimMontage*, bool)
//         {
//             CurrentLedge = NewLedgeInfo;
//             SnapToLedge(CurrentLedge);
//             CurrentState = ELedgeHangState::Hanging;
//         });
//         GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, CornerMontage);
//     }
//     else
//     {
//         // No montage authored — just snap directly
//         CurrentLedge = NewLedgeInfo;
//         SnapToLedge(CurrentLedge);
//         CurrentState = ELedgeHangState::Hanging;
//     }
// }
//
// void UPxiiLedgeTraversal::TryClimbUp()
// {
//     if (CurrentState != ELedgeHangState::Hanging) return;
//
//     CurrentState = ELedgeHangState::ClimbingUp;
//
//     if (MotionWarpingComp)
//     {
//         FVector TopSurface = CurrentLedge.EdgeLocation + FVector(0, 0, CapsuleComp->GetScaledCapsuleHalfHeight());
//         MotionWarpingComp->AddOrUpdateWarpTargetFromLocation(FName("ClimbTarget"), TopSurface);
//     }
//
//     if (GetAnimInstance() && ClimbUpMontage)
//     {
//         GetAnimInstance()->Montage_Play(ClimbUpMontage);
//
//         FOnMontageEnded EndDelegate;
//         EndDelegate.BindUObject(this, &UPxiiLedgeTraversal::OnClimbMontageEnded);
//         GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, ClimbUpMontage);
//     }
// }
//
// void UPxiiLedgeTraversal::OnClimbMontageEnded(UAnimMontage* Montage, bool bInterrupted)
// {
//     CurrentState = ELedgeHangState::None;
//     //MovementComp->SetMovementMode(MOVE_Walking);
// }
//
// void UPxiiLedgeTraversal::DropOff()
// {
//     if (CurrentState != ELedgeHangState::Hanging && CurrentState != ELedgeHangState::Shimmying) return;
//
//     CurrentState = ELedgeHangState::None;
//     //MovementComp->SetMovementMode(MOVE_Falling);
// }
//
// ELedgeHangState UPxiiLedgeTraversal::GetState() const
// {
//     return CurrentState; 
// }
//
// FVector UPxiiLedgeTraversal::GetWallLocation()
// {
//     if (CurrentState != ELedgeHangState::Hanging && CurrentState != ELedgeHangState::Shimmying)
//     {
//         return FVector::ZeroVector;   
//     }
//
//     return CurrentLedge.WallNormal;
// }
//
// FVector UPxiiLedgeTraversal::GetLedgeLocation() const
// {
//     if (CurrentState != ELedgeHangState::Hanging && CurrentState != ELedgeHangState::Shimmying)
//     {
//         return FVector::ZeroVector;   
//     }
//     
//     return CurrentLedge.EdgeLocation;
// }
//
// FVector UPxiiLedgeTraversal::GetAdjustedLedgeLocation(FLedgeHangInfo ledgeInfo) const
// {
//     return ledgeInfo.EdgeLocation
//         + (ledgeInfo.WallNormal * CapsuleRadius)
//         + FVector(0, 0, HangHandOffsetZ - CapsuleHalfHeight);
// }
//
// FLedgeHangInfo UPxiiLedgeTraversal::GetCurrentLedge() const
// {
//     return CurrentLedge;
// }
