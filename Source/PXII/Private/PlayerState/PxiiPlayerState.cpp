#include "PlayerState/PxiiPlayerState.h"

void APxiiPlayerState::AddExperience(int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	Experience += Amount;

	// Simple example threshold.
	while (Experience >= ExperienceToNextLevel)
	{
		LevelUp();
	}

	OnPlayerDataChanged.Broadcast();
}

void APxiiPlayerState::SetLevel(int32 NewLevel)
{
	PlayerLevel = FMath::Max(1, NewLevel);
	OnPlayerDataChanged.Broadcast();
}

void APxiiPlayerState::LoadSaveData(FPlayerData playerData)
{
	PlayerLevel = playerData.Level;
	Experience = playerData.Experience;
	Gold = playerData.Gold;
}

void APxiiPlayerState::FillSaveData(FPlayerData& OutData)
{
	OutData.Level = PlayerLevel;
	OutData.Experience = Experience;
	OutData.Gold = Gold;
}

void APxiiPlayerState::LevelUp()
{
	ExperienceToNextLevel += 500; //TEMP VALUE
	
	PlayerLevel++;
	OnPlayerDataChanged.Broadcast();
}
