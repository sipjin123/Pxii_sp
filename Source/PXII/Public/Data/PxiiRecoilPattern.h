#pragma once
#include "Animation/AnimMontage.h"
#include "Engine/DataAsset.h"
#include "PxiiRecoilPattern.generated.h"

UCLASS(BlueprintType)
class UPxiiRecoilPattern : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Recoil|Pattern")
	TArray<FVector2D> KickPattern;

	UPROPERTY(EditDefaultsOnly, Category = "Recoil|Pattern")
	float PatternScale = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Recoil|Pattern")
	float HorizontalRandomness = 0.15f;

	UPROPERTY(EditDefaultsOnly, Category = "Recoil|Camera")
	float CameraKickSpeed = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Recoil|Camera")
	float CameraRecoverySpeed = 6.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Recoil|Camera")
	float CameraRecoveryDelay = 0.1f; 

	UPROPERTY(EditDefaultsOnly, Category = "Recoil|Weapon")
	FVector WeaponKickOffset = FVector(-4.0f, 0.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Recoil|Weapon")
	FRotator WeaponKickRotation = FRotator(-8.0f, 0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Recoil|Weapon")
	float WeaponKickSpeed = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Recoil|Weapon")
	float WeaponRecoverySpeed = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Recoil|Animation")
	UAnimMontage* RecoilAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Recoil|Animation")
	float AnimRecoilPlayRate = 1.0f;
};
