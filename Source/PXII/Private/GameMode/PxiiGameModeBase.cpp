#include "GameMode/PxiiGameModeBase.h"

#include "Character/PxiiCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "SaveData/PxiiSaveData.h"

APxiiGameModeBase::APxiiGameModeBase()
{
	
}

EPxiiGamePhase APxiiGameModeBase::GetGamePhase() const
{
	return CurrentGamePhase;
}

void APxiiGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SetGamePhase(EPxiiGamePhase::Playing);
}

void APxiiGameModeBase::SetGamePhase(EPxiiGamePhase NewPhase)
{
	if (CurrentGamePhase == NewPhase)
	{
		return;
	}

	CurrentGamePhase = NewPhase;

	OnGamePhaseChanged.Broadcast(CurrentGamePhase);
}

APawn* APxiiGameModeBase::SpawnPlayer()
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return nullptr;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
	if (!PlayerController)
	{
		return nullptr;
	}
	
	AActor* StartSpot = FindPlayerStart(PlayerController);

	if (!StartSpot)
	{
		return nullptr;
	}

	if (!DefaultPawnClass)
	{
		return nullptr;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PlayerController;
	SpawnParams.Instigator = nullptr;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* Pawn = World->SpawnActor<APawn>(DefaultPawnClass, StartSpot->GetActorTransform(), SpawnParams);
	
	if (!Pawn)
	{
		return nullptr;
	}

	PlayerController->Possess(Pawn);
	return Pawn;
}

void APxiiGameModeBase::SaveCheckpoint()
{
	APxiiCharacter* Player = Cast<APxiiCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (!Player)
	{
		return;
	}

	UPxiiSaveData* saveData = Cast<UPxiiSaveData>(UGameplayStatics::CreateSaveGameObject(UPxiiSaveData::StaticClass()));
	FCheckpointData data = saveData->Checkpoint;
	data.PlayerLocation = Player->GetActorLocation();
	data.PlayerRotation = Player->GetActorRotation();

	saveData->Checkpoint = data;
	UGameplayStatics::SaveGameToSlot(saveData, TEXT("Checkpoint"), 0);
	
	UE_LOG(LogTemp, Log, TEXT("Checkpoint Saved | Location: %s | Rotation: %s"), *Player->GetActorLocation().ToString(), *Player->GetActorRotation().ToString());
}

void APxiiGameModeBase::LoadCheckpoint()
{
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("Checkpoint"), 0))
	{
		return;
	}

	UPxiiSaveData* SaveGame = Cast<UPxiiSaveData>(UGameplayStatics::LoadGameFromSlot(TEXT("Checkpoint"), 0));

	if (!SaveGame)
	{
		return;
	}

	APxiiCharacter* Player = Cast<APxiiCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!Player)
	{
		return;
	}
	FCheckpointData data = SaveGame->Checkpoint;
	Player->SetActorLocation(data.PlayerLocation);
	Player->SetActorRotation(data.PlayerRotation);

	UE_LOG(LogTemp, Log, TEXT("Checkpoint Loaded"));
}
