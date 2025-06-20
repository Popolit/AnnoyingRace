#include "RaceGameMode.h"

#include "RaceGameState.h"

ARaceGameMode::ARaceGameMode()
{
	GameStateClass = ARaceGameState::StaticClass();
}

void ARaceGameMode::StartPlay()
{
	ShuffleCharacterQueue();
}

void ARaceGameMode::StartRaceCountDown()
{
}

void ARaceGameMode::RemoveBlockVolumes()
{
}

void ARaceGameMode::ShuffleCharacterQueue()
{
	UINT LeftCount = CharacterPool_.Num();
	TArray<bool> IsSelected = { false, };
	while(LeftCount--)
	{
		UINT Index = FMath::RandRange(0, LeftCount);
		while(IsSelected[Index])
		{
			Index = (Index + 1) % CharacterPool_.Num();
		}
		IsSelected[Index] = true;
		CharacterQueue_.Enqueue(CharacterPool_[Index]);
	}
}

void ARaceGameMode::ChangeCharacter()
{
	
}
