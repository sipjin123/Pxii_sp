#pragma once
#include "GameFramework/PlayerState.h"
#include "SaveData/PxiiSaveData.h"
#include "PxiiPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPxiiPlayerDataChangedSignature);

UCLASS(Blueprintable)
class PXII_API APxiiPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FPxiiPlayerDataChangedSignature OnPlayerDataChanged;

	UPROPERTY(BlueprintReadOnly)
	int32 PlayerLevel = 1;

	UPROPERTY(BlueprintReadOnly)
	int32 Experience = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 Gold = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 ExperienceToNextLevel = 0;
	
	UFUNCTION(BlueprintCallable, Category = "Progression")
	virtual void AddExperience(int32 Amount);
	
	UFUNCTION(BlueprintCallable, Category = "Progression")
	virtual void SetLevel(int32 NewLevel);

	virtual void LoadSaveData(FPlayerData playerData);

	virtual void FillSaveData(FPlayerData& OutData);

protected:

	virtual void LevelUp();
};
