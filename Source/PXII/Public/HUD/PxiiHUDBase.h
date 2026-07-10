#pragma once
#include "PxiiHUD.h"
#include "Character/PxiiCharacter.h"
#include "GameFramework/HUD.h"
#include "PlayerController/PxiiPlayerController.h"
#include "Widgets/PxiiWidgetStackBase.h"
#include "Subsystem/PxiiUISubsystem.h"
#include "PxiiHUDBase.generated.h"

UCLASS(Blueprintable)
class PXII_API APxiiHUDBase : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UPxiiWidgetStackBase> StackLayoutClass;

	UPROPERTY(BlueprintReadWrite, Category="UI")
	TObjectPtr<UPxiiHUD> HUDLayout;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	TObjectPtr<UPxiiWidgetStackBase> StackLayout;

	UPROPERTY(BlueprintReadOnly, Category="References")
	TObjectPtr<APxiiPlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="References")
	TObjectPtr<APxiiCharacter> PlayerCharacter;
	
	UPROPERTY(BlueprintReadOnly, Category = "References")
	UPxiiUISubsystem* UISubsystem;

protected:
	virtual void CacheReferences();
};
