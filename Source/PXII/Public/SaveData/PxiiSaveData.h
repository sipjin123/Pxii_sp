#pragma once
#include "GameFramework/SaveGame.h"
#include "PxiiSaveData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int32 Level = 1;

	UPROPERTY()
	int32 Experience = 0;

	UPROPERTY()
	int32 Gold = 0;
};

USTRUCT(BlueprintType)
struct FCheckpointData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FVector PlayerLocation;

	UPROPERTY()
	FRotator PlayerRotation;
};

UCLASS()
class PXII_API UPxiiSaveData : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FPlayerData Player;

	UPROPERTY()
	FCheckpointData Checkpoint;
};
