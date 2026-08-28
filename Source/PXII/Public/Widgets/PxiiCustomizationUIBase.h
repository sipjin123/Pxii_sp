#pragma once
#include "CommonUI/PxiiActivatableWidget.h"
#include "PxiiCustomizationUIBase.generated.h"

UCLASS(Blueprintable)
class PXII_API UPxiiCustomizationUIBase : public UPxiiActivatableWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void RotateRight(float progress);
	UFUNCTION()
	void RotateLeft(float progress);
	UFUNCTION()
	void MoveUp(float progress);
	UFUNCTION()
	void MoveDown(float progress);
	UFUNCTION()
	void ZoomIn(float progress);
	UFUNCTION()
	void ZoomOut(float progress);

	UFUNCTION()
	void DummyFunction();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnBackBoundActionTriggered();
	void OnBackBoundActionTriggered_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void VerticalCameraMovement(bool moveUp);
	void VerticalCameraMovement_Implementation(bool moveUp);

	UFUNCTION(BlueprintNativeEvent)
	void ZoomCameraMovement(bool zoomIn);
	void ZoomCameraMovement_Implementation(bool zoomIn);

	UFUNCTION(BlueprintNativeEvent)
	void RotationCameraMovement(bool rotateRight);
	void RotationCameraMovement_Implementation(bool rotateRight);

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	FDataTableRowHandle MoveUpKey;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	FDataTableRowHandle MoveDownKey;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	FDataTableRowHandle ZoomInKey;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	FDataTableRowHandle ZoomOutKey;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	FDataTableRowHandle RotateLeftKey;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	FDataTableRowHandle RotateRightKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ZoomIA;
};
