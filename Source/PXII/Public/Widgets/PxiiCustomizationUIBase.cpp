#include "PxiiCustomizationUIBase.h"
#include "ICommonInputModule.h"
#include "Input/CommonUIInputTypes.h"

void UPxiiCustomizationUIBase::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UPxiiCustomizationUIBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	auto backDelegate = FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered);
	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true, backDelegate)
	);

	auto moveUpdelegate = FSimpleDelegate::CreateUObject(this, &UPxiiCustomizationUIBase::DummyFunction);
	auto moveUpBinding = FBindUIActionArgs(MoveUpKey,true, moveUpdelegate);
	moveUpBinding.OnHoldActionProgressed.BindUObject(this, &ThisClass::MoveUp);
	RegisterUIActionBinding(moveUpBinding);

	auto  moveDownDelegate = FSimpleDelegate::CreateUObject(this, &UPxiiCustomizationUIBase::DummyFunction);
	auto moveDownBinding = FBindUIActionArgs(MoveDownKey,true, moveDownDelegate);
	moveDownBinding.OnHoldActionProgressed.BindUObject(this, &ThisClass::MoveDown);
	RegisterUIActionBinding(moveDownBinding);

	auto rotLeftDelegate = FSimpleDelegate::CreateUObject(this, &UPxiiCustomizationUIBase::DummyFunction);
	auto rotLeftBinding = FBindUIActionArgs(RotateLeftKey,true, rotLeftDelegate);
	rotLeftBinding.OnHoldActionProgressed.BindUObject(this, &ThisClass::RotateLeft);
	RegisterUIActionBinding(rotLeftBinding);

	auto rotRightDelegate = FSimpleDelegate::CreateUObject(this, &UPxiiCustomizationUIBase::DummyFunction);
	auto rotRightBinding = FBindUIActionArgs(RotateRightKey,true, rotRightDelegate);
	rotRightBinding.OnHoldActionProgressed.BindUObject(this, &ThisClass::RotateRight);
	RegisterUIActionBinding(rotRightBinding);

	auto zoomInDelegate = FSimpleDelegate::CreateUObject(this, &UPxiiCustomizationUIBase::DummyFunction);
	auto zoomInBinding = FBindUIActionArgs(ZoomInKey,true, zoomInDelegate);
	zoomInBinding.OnHoldActionProgressed.BindUObject(this, &ThisClass::ZoomIn);
	RegisterUIActionBinding(zoomInBinding);

	auto zoomOutdelegate = FSimpleDelegate::CreateUObject(this, &UPxiiCustomizationUIBase::DummyFunction);
	auto zoomOutBinding = FBindUIActionArgs(ZoomOutKey,true, zoomOutdelegate);
	zoomOutBinding.OnHoldActionProgressed.BindUObject(this, &ThisClass::ZoomOut);
	RegisterUIActionBinding(zoomOutBinding);

}

void UPxiiCustomizationUIBase::RotateRight(float progress)
{
	RotationCameraMovement(true);
}

void UPxiiCustomizationUIBase::RotateLeft(float progress)
{
	RotationCameraMovement(false);
}

void UPxiiCustomizationUIBase::MoveUp(float progress)
{
	VerticalCameraMovement(true);
}

void UPxiiCustomizationUIBase::MoveDown(float progress)
{
	VerticalCameraMovement(false);

}

void UPxiiCustomizationUIBase::ZoomIn(float progress)
{
	ZoomCameraMovement(true);
}

void UPxiiCustomizationUIBase::ZoomOut(float progress)
{
	ZoomCameraMovement(false);
}

void UPxiiCustomizationUIBase::DummyFunction()
{
}

void UPxiiCustomizationUIBase::OnBackBoundActionTriggered_Implementation()
{
	DeactivateWidget();
}

void UPxiiCustomizationUIBase::VerticalCameraMovement_Implementation(bool moveUp)
{
	
}

void UPxiiCustomizationUIBase::ZoomCameraMovement_Implementation(bool zoomIn)
{
	
}

void UPxiiCustomizationUIBase::RotationCameraMovement_Implementation(bool rotateRight)
{
	
}
