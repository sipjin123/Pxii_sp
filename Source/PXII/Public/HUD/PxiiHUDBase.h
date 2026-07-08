#pragma once
#include "PxiiHUD.h"
#include "Character/PxiiCharacter.h"
#include "GameFramework/HUD.h"
#include "PlayerController/PxiiPlayerController.h"
#include "PxiiHUDBase.generated.h"

UCLASS(Blueprintable)
class PXII_API APxiiHUDBase : public AHUD
{
	GENERATED_BODY()
	
public:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<UPxiiHUD> HUDLayoutClass;

	UPROPERTY(BlueprintReadOnly, Category="UI")
	TObjectPtr<UPxiiHUD> HUDLayout;

	UPROPERTY(BlueprintReadOnly, Category="References")
	TObjectPtr<APxiiPlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="References")
	TObjectPtr<APxiiCharacter> PlayerCharacter;
	
protected:
	virtual void CacheReferences();
	virtual void CreateHUDLayout();

};
