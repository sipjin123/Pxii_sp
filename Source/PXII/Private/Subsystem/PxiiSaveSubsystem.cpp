#include "Subsystem/PxiiSaveSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "PlayerState/PxiiPlayerState.h"

void UPxiiSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UPxiiSaveSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UPxiiSaveSubsystem::SaveGame()
{
	if (!CreateSaveIfNeeded())
	{
		return false;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!PC)
	{
		return false;
	}

	APxiiPlayerState* PlayerState = PC->GetPlayerState<APxiiPlayerState>();

	if (PlayerState)
	{
		PlayerState->FillSaveData(CurrentSaveData->Player);
	}

	return UGameplayStatics::SaveGameToSlot(CurrentSaveData, SaveSlot, UserIndex);
}

bool UPxiiSaveSubsystem::LoadGame()
{
	if (!CreateSaveIfNeeded())
	{
		return false;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!PC)
	{
		return false;
	}

	APxiiPlayerState* PlayerState = PC->GetPlayerState<APxiiPlayerState>();
	if (PlayerState)
	{
		PlayerState->LoadSaveData(CurrentSaveData->Player);
	}

	return true;
}

UPxiiSaveData* UPxiiSaveSubsystem::GetCurrentSave() const
{
	return CurrentSaveData;
}

bool UPxiiSaveSubsystem::SaveCustomization(UPxiiCustomizationComponent* comp)
{
	if (!CreateSaveIfNeeded())
	{
		return false;
	}

	if(!comp)
	{
		return false;
	}

	comp->FillCustomizationSaveData(CurrentSaveData->Customization);
	SaveGame();
	
	return true;
}

bool UPxiiSaveSubsystem::LoadCustomization(UPxiiCustomizationComponent* comp)
{
	if (!CreateSaveIfNeeded())
	{
		return false;
	}

	if(!comp)
	{
		return false;
	}
	
	comp->LoadData(CurrentSaveData->Customization);
	return true;
}

bool UPxiiSaveSubsystem::CreateSaveIfNeeded()
{
	if (CurrentSaveData)
	{
		return true;
	}

	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, UserIndex))
	{
		CurrentSaveData = Cast<UPxiiSaveData>(UGameplayStatics::LoadGameFromSlot(SaveSlot, UserIndex));
	}
	else
	{
		CurrentSaveData = Cast<UPxiiSaveData>(UGameplayStatics::CreateSaveGameObject(UPxiiSaveData::StaticClass()));
	}

	return CurrentSaveData != nullptr;
}
