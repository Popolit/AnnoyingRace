#include "RaceGameState.h"

#include "RacePlayerState.h"
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
	DOREPLIFETIME(ARaceGameState, PlayerRankings_);
}

void ARaceGameState::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	if(HasAuthority())
	{
		GetWorldTimerManager().SetTimer(RankingUpdateTimerHandle, this, &ARaceGameState::UpdatePlayerRankings, 0.2f, true);
	}
}


void ARaceGameState::HandleStartRace()
{
	if (HasAuthority())
	{
		RaceStartTime_ = GetServerWorldTimeSeconds();
	}
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

const TArray<TObjectPtr<APlayerState>>& ARaceGameState::GetPlayerRankings() const
{
	return PlayerRankings_;
}

//This Function Executed On Server Only
void ARaceGameState::UpdatePlayerRankings()
{
	PlayerRankings_ = PlayerArray;

	PlayerRankings_.Sort([](const APlayerState& _Lhs, const APlayerState& _Rhs)
		{
			const auto LhsState = Cast<ARacePlayerState>(&_Lhs);
			const auto RhsState = Cast<ARacePlayerState>(&_Rhs);

			//존재하지 않는 (탈주 등) 유저는 순위에서 밀림
			if (!LhsState)
			{
				return false;
			}
			if (!RhsState)
			{
				return true;
			}

			//Laps가 같으면 진행거리로 판단
			if (LhsState->GetLaps() == RhsState->GetLaps())
			{
				return LhsState->GetTotalDistance() > RhsState->GetTotalDistance();
			}

			return LhsState->GetLaps() > RhsState->GetLaps();
		});

	OnRep_PlayerRankings();
}

void ARaceGameState::OnRep_PlayerRankings()
{
	OnPlayerRankingUpdated_.ExecuteIfBound();
}
