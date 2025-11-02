#include "RaceGameInstance.h"

#include "CommonSessionSubsystem.h"
#include "GameFramework/PlayerState.h"

#define LOCTEXT_NAMESPACE "ErrorMessages"

void URaceGameInstance::Init()
{
	Super::Init();
	
	SessionSubsystem_ = GetSubsystem<UCommonSessionSubsystem>();
	if (ensure(SessionSubsystem_))
	{
		SessionSubsystem_->OnCreateSessionCompleteEvent.AddUObject(this, &URaceGameInstance::OnCreateSessionComplete);
		SessionSubsystem_->OnJoinSessionCompleteEvent.AddUObject(this, &URaceGameInstance::OnJoinSessionComplete);
	}

	GEngine->OnNetworkFailure().AddUObject(this, &URaceGameInstance::HandleNetworkFailure);
}

void URaceGameInstance::Shutdown()
{
	if (SessionSubsystem_)
	{
		SessionSubsystem_->CleanUpSessions();
	}
	Super::Shutdown();
}

void URaceGameInstance::CreateSession(APlayerController* _PC, const FSessionData& _SessionData)
{
	if (_PC && SessionSubsystem_)
	{
		UCommonSession_HostSessionRequest* SessionRequest = SessionSubsystem_->CreateOnlineHostSessionRequest();
		if (SessionRequest && ensureMsgf(SessionMap_.IsValid(), TEXT("Session Map was not set")))
		{
			SessionRequest->bUseLobbies = true;
			SessionRequest->bUsePresence = true;
			SessionRequest->MaxPlayerCount = _SessionData.MaxUserCount_;
			SessionRequest->MapID = SessionMap_;
			SessionRequest->SessionName = _SessionData.SessionName_;
			SessionRequest->MapData = _SessionData.MapData_;
			SessionSubsystem_->HostSession(_PC, SessionRequest);
		}
	}
}

void URaceGameInstance::FindSessions(APlayerController* _PC)
{
	if (_PC && SessionSubsystem_)
	{
		SearchSessionRequest_ = SessionSubsystem_->CreateOnlineSearchSessionRequest();
		if (ensureMsgf(SearchSessionRequest_, TEXT("Search Session Request was nullptr")))
		{
			SearchSessionRequest_->OnSearchFinished.AddUObject(this, &URaceGameInstance::OnFindSessionsComplete);
			SessionSubsystem_->FindSessions(_PC, SearchSessionRequest_);
		}
	}
}

void URaceGameInstance::JoinSessionWithResult(APlayerController* _PC, UCommonSession_SearchResult* _SearchResult)
{
	if (_PC && SessionSubsystem_)
	{
		SessionSubsystem_->JoinSession(_PC, _SearchResult);
	}
}

void URaceGameInstance::CleanUpSession(APlayerController* _PC)
{
	if (_PC && SessionSubsystem_)
	{
		SessionSubsystem_->CleanUpSessions();
		if (ensureMsgf(LobbyMap_.IsValid(), TEXT("Lobby Map was not set")))
		{
			_PC->ClientTravel(LobbyMap_.ToString(), TRAVEL_Absolute, true);
		}
	}
}

FPrimaryAssetId URaceGameInstance::GetSessionMap() const
{
	return SessionMap_;
}

void URaceGameInstance::SetRacePlayerCount(const int32 _RacePlayerCount)
{
	RacePlayerCount_ = _RacePlayerCount;
}

int32 URaceGameInstance::GetRacePlayerCount() const 
{
	return RacePlayerCount_;
}

void URaceGameInstance::SetRaceLaps(const int32 _RaceLaps)
{
	RaceLaps_ = _RaceLaps;
}

uint8 URaceGameInstance::GetRaceLaps() const
{
	return RaceLaps_;
}

void URaceGameInstance::SetPrevGameResult(const TArray<TObjectPtr<APlayerState>>& _PlayerRankings)
{
	if (false == PrevRaceGameResult_.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("PrevRaceGameResult is not empty"));
		return;
	}
	
	int32 Rank = 1;
	for (const auto& PS :  _PlayerRankings)
	{
		PrevRaceGameResult_.Emplace(PS->GetPlayerName(), Rank++);
	}
}

bool URaceGameInstance::CheckPrevGameWasExist() const
{
	return !(PrevRaceGameResult_.IsEmpty());
}

TArray<FRaceGameResultData> URaceGameInstance::GetPrevGameResult() const
{
	return PrevRaceGameResult_;
}

void URaceGameInstance::ClearPrevGameResult()
{
	PrevRaceGameResult_.Empty();
}

void URaceGameInstance::OnCreateSessionComplete(const FOnlineResultInformation& _Result)
{
	if (false == _Result.bWasSuccessful)
	{
		FText ErrorReason = _Result.ErrorText.IsEmpty() ? LOCTEXT("UnknownError", "Unknown Error") : _Result.ErrorText; 
		FText ErrorMessage = FText::Format(LOCTEXT("Error_CreateSessionFailure", "Failed to create Session.\nReason: {0}"), 
			ErrorReason);
		OnSessionError_.ExecuteIfBound(_Result.ErrorText);
	}
}


void URaceGameInstance::OnJoinSessionComplete(const FOnlineResultInformation& _Result)
{
	if (false == _Result.bWasSuccessful)
	{
		FText ErrorReason = _Result.ErrorText.IsEmpty() ? LOCTEXT("UnknownError", "Unknown Error") : _Result.ErrorText; 
		FText ErrorMessage = FText::Format(LOCTEXT("Error_JoinSessionFailure", "Failed to join Session.\nReason: {0}"), 
			ErrorReason);
		OnSessionError_.ExecuteIfBound(ErrorMessage);
	}
}

void URaceGameInstance::OnFindSessionsComplete(bool _bSuccess, const FText& _ErrorMessage)
{
	if (SearchSessionRequest_)
	{
		OnSessionFindComplete_.ExecuteIfBound(_bSuccess, _ErrorMessage, SearchSessionRequest_->Results);
	}
}

void URaceGameInstance::HandleNetworkFailure(UWorld* _World, UNetDriver* _NetDriver, ENetworkFailure::Type _FailureType,
	const FString& _ErrorString)
{
	PendingErrorMessage_ = FText::Format(LOCTEXT("Error_NetworkFailure", "Network Error\nReason: {0}"), 
		FText::FromString(_ErrorString)
	);
}
#undef LOCTEXT_NAMESPACE
