#include "RaceGameState.h"

#include "RacePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ARaceGameState::ARaceGameState()
{
	bRaceStarted_ = false;
	MaxLap_ = 3;
	ElapsedTime_ = 0;
}

void ARaceGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARaceGameState, CountdownStartTime_);
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

bool ARaceGameState::IsRaceStarted()
{
	return bRaceStarted_;
}

void ARaceGameState::OnRep_CountdownStartTime()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (ARacePlayerController* RPC = Cast<ARacePlayerController>(PC))
	{
		//RPC->StartRaceCountdown(CountdownStartTime_);
	}
}
