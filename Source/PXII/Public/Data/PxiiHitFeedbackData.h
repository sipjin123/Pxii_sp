#pragma once
#include "Engine/DataAsset.h"
#include "Camera/CameraShakeBase.h"
#include "Engine/Texture2D.h"
#include "PxiiHitFeedbackData.generated.h"

UENUM(BlueprintType)
enum class EHitFeedbackType : uint8
{
	Standard	UMETA(DisplayName = "Standard Hit"),
	Critical	UMETA(DisplayName = "Critical/Headshot"),
	Kill		UMETA(DisplayName = "Kill"),
	Blocked		UMETA(DisplayName = "Blocked"),
	Friendly	UMETA(DisplayName = "Friendly")
};

USTRUCT(BlueprintType)
struct FHitFeedbackEntry
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Feedback|Visual")
	TObjectPtr<UTexture2D> Hitmarkers;
	
	UPROPERTY(EditDefaultsOnly, Category = "Feedback|Visual")
	FLinearColor HitmarkerColor = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, Category = "Feedback|Visual")
	float HitmarkerScale = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Feedback|Visual")
	float HitmarkerDuration = 0.15f;

	UPROPERTY(EditDefaultsOnly, Category = "Feedback|Audio")
	USoundBase* HitmarkerSfx;

	UPROPERTY(EditDefaultsOnly, Category = "Feedback|Audio")
	float HitmarkerSfxVolumeMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = "Feedback|Camera")
	TSubclassOf<class UCameraShakeBase> ShooterCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Feedback|DamageNumber")
	bool bShowDamageNumber = true;

	UPROPERTY(EditDefaultsOnly, Category = "Feedback|DamageNumber")
	FLinearColor DamageNumberColor = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, Category = "Feedback|DamageNumber")
	float DamageNumberScale = 1.0f;
	
};

UCLASS(BlueprintType)
class PXII_API UPxiiHitFeedbackData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Feedback")
	TMap<EHitFeedbackType, FHitFeedbackEntry> FeedbackEntries;

	const FHitFeedbackEntry* GetFeedback(EHitFeedbackType Type) const;
	const bool HasFeedback(EHitFeedbackType Type) const;

};
