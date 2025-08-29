#include "RaceGameInstance.h"

#include "SessionData.h"
#include "CommonSessionSubsystem.h"
#include "LobbyPlayerController.h"

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
			SessionRequest->bIsPrivate = _SessionData.bPrivate_;
			SessionRequest->Password = _SessionData.Password_;
			SessionSubsystem_->HostSession(_PC, SessionRequest);
		}
	}
}

void URaceGameInstance::FindSessions(APlayerController* _PC, const FSessionFindData& _SessionFindData)
{
	if (_PC && SessionSubsystem_)
	{
		SearchSessionRequest_ = SessionSubsystem_->CreateOnlineSearchSessionRequest();
		if (ensureMsgf(SearchSessionRequest_, TEXT("Search Session Request was nullptr")))
		{
			SearchSessionRequest_->OnSearchFinished.AddUObject(this, &URaceGameInstance::OnFindSessionsComplete);
			SearchSessionRequest_->SessionName = _SessionFindData.SessionName_;
			SearchSessionRequest_->MapData = _SessionFindData.MapData_;
			SearchSessionRequest_->MaxUserCount = _SessionFindData.MaxUserCount_;
			SearchSessionRequest_->MinUserCount = _SessionFindData.MinUserCount_;
			SessionSubsystem_->FindSessions(_PC, SearchSessionRequest_);
		}
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

void URaceGameInstance::SetRacePlayerCount(int32 _RacePlayerCount)
{
	RacePlayerCount_ = _RacePlayerCount;
}

int32 URaceGameInstance::GetRacePlayerCount() const
{
	return RacePlayerCount_;
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
