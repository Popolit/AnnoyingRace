#include "SessionGameState.h"

#include "SessionPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

ASessionGameState::ASessionGameState()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	//최소값을 일단 부여
	PlayerList_.SetNum(1);
}

void ASessionGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASessionGameState, SessionInfo_);
	DOREPLIFETIME(ASessionGameState, PlayerList_);
	DOREPLIFETIME(ASessionGameState, HostId_);
	DOREPLIFETIME(ASessionGameState, PrevGameResult_);
}

void ASessionGameState::UpdateSession(const FSessionInfo& _SessionInfo)
{
	if (HasAuthority())
	{
		SessionInfo_ = _SessionInfo;
		PlayerList_.SetNum(SessionInfo_.MaxUserCount_);
		OnRep_SessionInfo();
	}
}

const FSessionInfo& ASessionGameState::GetSessionInfo() const
{
	return SessionInfo_;
}


void ASessionGameState::AddPlayer(const APlayerState* _PlayerState)
{
	if (HasAuthority() && _PlayerState)
	{
		FSessionPlayerInfo NewPlayerInfo;

		NewPlayerInfo.UniqueNetId_ = _PlayerState->GetUniqueId();
		NewPlayerInfo.PlayerName_ = _PlayerState->GetPlayerName();
		NewPlayerInfo.bIsReady_ = false;
		NewPlayerInfo.bIsHost_ = false;

		for (FSessionPlayerInfo& PlayerInfo : PlayerList_)
		{
			if (false == PlayerInfo.UniqueNetId_.IsValid())
			{
				PlayerInfo = NewPlayerInfo;
				break;
			}
		}
		OnRep_SessionInfo();
		OnRep_PlayerList();
	}
}

void ASessionGameState::RemovePlayer(const APlayerState* _PlayerState)
{
	if (HasAuthority() && _PlayerState)
	{
		int8 Index = FindPlayerIndex(_PlayerState->GetUniqueId());
		if (PlayerList_.IsValidIndex(Index))
		{
			PlayerList_[Index].UniqueNetId_ = nullptr;
			OnRep_PlayerList();
		}
	}
}

void ASessionGameState::TogglePlayerIsReady(const APlayerState* _PlayerState)
{
	if (HasAuthority() && _PlayerState)
	{
		int8 Index = FindPlayerIndex(_PlayerState->GetUniqueId());
		if (PlayerList_.IsValidIndex(Index) && PlayerList_[Index].UniqueNetId_.IsValid())
		{
			PlayerList_[Index].bIsReady_ = !PlayerList_[Index].bIsReady_;
			OnRep_PlayerList();
			CheckAllPlayersReady();
		}
	}
}

void ASessionGameState::SetHost(const FUniqueNetIdRepl& _HostId)
{
	if (HasAuthority())
	{
		//새 호스트 설정
		int8 Index = FindPlayerIndex(_HostId);
		if (false == PlayerList_.IsValidIndex(Index))
		{
			ensureMsgf(false, TEXT("Host Index was wrong"));
			return;
		}
		PlayerList_[Index].bIsHost_ = true;
		
		//이전 호스트 제거
		Index = FindPlayerIndex(HostId_);
		if (PlayerList_.IsValidIndex(Index))
		{
			PlayerList_[Index].bIsHost_ = false;
		}

		//HostID 교체		
		HostId_ = _HostId;
		OnRep_PlayerList();
	}
}

void ASessionGameState::SetLaps(const int32 _Laps)
{
	if (false == HasAuthority())
	{
		return;
	}
	if (_Laps <= 0 || 10 < _Laps)
	{
		ensureMsgf(false, TEXT("Laps was Invalid Value : %d"), _Laps);
		return;
	}

	SessionInfo_.Laps_ = _Laps;
	OnRep_SessionInfo();
}

TArray<FSessionPlayerInfo> ASessionGameState::GetPlayerList() const
{
	return PlayerList_;
}

void ASessionGameState::SetPrevGameResult(const TArray<FRaceGameResultData>& _PrevGameResult)
{
	PrevGameResult_ = _PrevGameResult;
	OnRep_PrevGameResult();
}

const TArray<FRaceGameResultData>& ASessionGameState::GetPrevGameResult() const
{
	return PrevGameResult_;
}

void ASessionGameState::CheckAllPlayersReady()
{
	if (false == HasAuthority())
	{
		return;
	}
	
	UWorld* World = GetWorld();
	if (nullptr == World)
	{
		return;
	}

	for (const FSessionPlayerInfo& PlayerInfo : PlayerList_)
	{
		if (false == PlayerInfo.UniqueNetId_.IsValid())
		{
			continue;
		}
		
		//준비 되지 않은 사람이 있으면, 카운트 다운 정지
		if (false == PlayerInfo.bIsReady_)
		{
			for (auto It = World->GetPlayerControllerIterator(); It; It++)
			{
				if (auto PC = Cast<ASessionPlayerController>(*It))
				{
					PC->Client_StopCountDownForPlay();
				}
			}
			return;
		}
	}

	//모두 준비되었으면, 카운트 다운 시작
	for (auto It = World->GetPlayerControllerIterator(); It; It++)
	{
		if (auto PC = Cast<ASessionPlayerController>(*It))
		{
			PC->Client_StartCountDownForPlay();
		}
	}
}

int8 ASessionGameState::FindPlayerIndex(const FUniqueNetIdRepl& _PlayerId) const
{
	if (_PlayerId.IsValid())
	{
		for (uint8 i = 0; i < PlayerList_.Num(); i++)
		{
			if (PlayerList_[i].UniqueNetId_ == _PlayerId)
			{
				return i;
			}
		}
	}
	return -1;
}

void ASessionGameState::OnRep_SessionInfo() const
{
	OnSessionInfoUpdated_.ExecuteIfBound(SessionInfo_);
}

void ASessionGameState::OnRep_PlayerList() const
{
	OnPlayerListUpdated_.ExecuteIfBound(PlayerList_);
}

void ASessionGameState::OnRep_PrevGameResult() const
{
	OnPrevGameResultUpdated_.ExecuteIfBound(PrevGameResult_);
}
