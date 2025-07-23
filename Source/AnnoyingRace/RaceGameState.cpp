#include "RaceGameState.h"

#include "LevelSequenceActor.h"
#include "Net/UnrealNetwork.h"


ARaceGameState::ARaceGameState()
{
	MaxLap_ = 3;
	ElapsedTime_ = 0.f;
}

void ARaceGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARaceGameState, RaceStartTime_);
	DOREPLIFETIME(ARaceGameState, SequencActor_);
}

void ARaceGameState::HandleStartRace()
{
	if (HasAuthority())
	{
		RaceStartTime_ = GetServerWorldTimeSeconds();
	}
}

void ARaceGameState::SetSequenceActor(ALevelSequenceActor* _SequenceActor)
{
	SequencActor_ = GetWorld()->SpawnActor<ALevelSequenceActor>();
	SequencActor_->SetReplicatePlayback(true);
}

ALevelSequenceActor* ARaceGameState::GetSequenceActor() const
{
	return SequencActor_;
}


void ARaceGameState::SetMaxCheckPointCount(uint8 _Count)
{
	MaxCheckPointCount_ = _Count;
}

uint8 ARaceGameState::GetMaxCheckPointCount() const
{
	return MaxCheckPointCount_;
}

void ARaceGameState::SetMaxLap(uint8 _MaxLaps)
{
	MaxLap_ = _MaxLaps;
}

uint8 ARaceGameState::GetMaxLap() const
{
	return MaxLap_;
}

float ARaceGameState::GetRaceElapsedTime() const
{
	if (ElapsedTime_ > 0.f)
	{
		return GetServerWorldTimeSeconds() - ElapsedTime_;
	}
	return 0.f;
}