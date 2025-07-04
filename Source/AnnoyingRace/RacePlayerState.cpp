#include "RacePlayerState.h"

ARacePlayerState::ARacePlayerState()
{
	Laps_ = 0;
	TargetCheckPointIndex_ = 0;
	Distance_ = 0;
}

uint8 ARacePlayerState::GetLaps() const
{
	return Laps_;
}

void ARacePlayerState::IncreaseLap()
{
	Laps_++;
}

uint8 ARacePlayerState::GetTargetCheckPointIndex() const
{
	return TargetCheckPointIndex_;
}

void ARacePlayerState::SetTargetCheckPointIndex(uint8 _Index)
{
	TargetCheckPointIndex_ = _Index;
}
