#pragma once
#include "PxiiHitFeedbackComponent.generated.h"

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Up          UMETA(DisplayName = "Up"),
	UpRight     UMETA(DisplayName = "Up Right"),
	Right       UMETA(DisplayName = "Right"),
	DownRight   UMETA(DisplayName = "Down Right"),
	Down        UMETA(DisplayName = "Down"),
	DownLeft    UMETA(DisplayName = "Down Left"),
	Left        UMETA(DisplayName = "Left"),
	UpLeft      UMETA(DisplayName = "Up Left"),
	Front		UMETA(DisplayName = "Front"),
	Back		UMETA(DisplayName = "Back")
};


UCLASS(Blueprintable)
class PXII_API UPxiiHitFeedbackComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EHitDirection, TObjectPtr<UAnimMontage>> HitReactAnimation;

	UPROPERTY(EditAnywhere, Category="Hit Feedback")
	FLinearColor HitFlashColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, Category="Hit Feedback")
	float HitFlashIntensity = 50.0f;

	UPROPERTY(EditAnywhere, Category="Hit Feedback")
	float HitFlashDuration = 0.2f;

	UPROPERTY(EditAnywhere, Category="Hit Feedback")
	float MaxOpacity = 1.0f;
	
	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayHitFlash();
	
	virtual void PlayHitFlash_Implementation();

	UFUNCTION(BlueprintCallable)
	void CreateDynamicMaterial(UMeshComponent* mesh);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateHitFlash();

	void UpdateHitFlash_Implementation();
	
private:
	

	FTimerHandle HitFlashTimerHandle;

	float HitFlashElapsed = 0.0f;

};
