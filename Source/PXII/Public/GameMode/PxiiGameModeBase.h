#pragma once
#include "GameFramework/GameModeBase.h"
#include "PxiiGameModeBase.generated.h"

UENUM(BlueprintType)
enum class EPxiiGamePhase : uint8
{
	None			UMETA(DisplayName = "None"),
	Loading			UMETA(DisplayName = "Loading"),
	Playing			UMETA(DisplayName = "Playing"),
	Paused			UMETA(DisplayName = "Paused"),
	GameOver		UMETA(DisplayName = "Game Over")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPxiiGamePhaseChangedSignature, EPxiiGamePhase, NewPhase);

UCLASS(Blueprintable)
class PXII_API APxiiGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APxiiGameModeBase();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Game")
	EPxiiGamePhase GetGamePhase() const;

	UFUNCTION(BlueprintCallable, Category = "Game")
	virtual void SetGamePhase(EPxiiGamePhase NewPhase);

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FPxiiGamePhaseChangedSignature OnGamePhaseChanged;

	UFUNCTION(BlueprintCallable, Category = "Game")
	virtual APawn* SpawnPlayer();
	
	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
	virtual void SaveCheckpoint();

	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
	virtual void LoadCheckpoint();

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Game")
	EPxiiGamePhase CurrentGamePhase = EPxiiGamePhase::None;

};
