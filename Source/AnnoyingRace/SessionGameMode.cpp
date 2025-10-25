#include "SessionGameMode.h"

#include "SessionGameState.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "SessionPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineSessionInterface.h"


ASessionGameMode::ASessionGameMode()
{
    GameStateClass = ASessionGameState::StaticClass();
    bUseSeamlessTravel = false;
}

void ASessionGameMode::BeginPlay()
{
    Super::BeginPlay();

    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ASessionGameMode::OnPostLoadMap);
}

void ASessionGameMode::PostLogin(APlayerController* _NewPlayer)
{
    Super::PostLogin(_NewPlayer);
    
    ASessionGameState* GS = GetGameState<ASessionGameState>();
    check(GS);

    GS->AddPlayer(_NewPlayer->PlayerState);
}

void ASessionGameMode::Logout(AController* _ExitPlayer)
{
    Super::Logout(_ExitPlayer);

    ASessionGameState* GS = GetGameState<ASessionGameState>();

    if (GS)
    {
        GS->RemovePlayer(_ExitPlayer->PlayerState);
    }
}

void ASessionGameMode::SetSessionJoinable(bool _bCanJoin)
{
    auto SessionInterface = GetSessionInterface();
    if (SessionInterface)
    {
        FNamedOnlineSession* Session = SessionInterface->GetNamedSession(NAME_GameSession);
        if (Session)
        {
            Session->SessionSettings.bAllowJoinInProgress = _bCanJoin;
            SessionInterface->UpdateSession(NAME_GameSession, Session->SessionSettings);
        }
    }
}

//공개 세션으로 변경
void ASessionGameMode::MakeSessionPublic()
{
	auto SessionInterface = GetSessionInterface();
	if (SessionInterface)
	{
		FNamedOnlineSession* Session = SessionInterface->GetNamedSession(NAME_GameSession);
		if (Session)
		{
			int32 NumUsers = Session->SessionSettings.NumPrivateConnections;
			Session->SessionSettings.NumPublicConnections = NumUsers + 1;
			Session->SessionSettings.NumPrivateConnections = 0;
			SessionInterface->UpdateSession(NAME_GameSession, Session->SessionSettings);
		}
	}
}

void ASessionGameMode::RequestChat(const FText& _Chat, const FString& _ChatterName)
{
    ASessionGameState* GS = GetGameState<ASessionGameState>();
    if (GS)
    {
        for (APlayerState* PS : GS->PlayerArray)
        {
            auto PC = Cast<ASessionPlayerController>(PS->GetPlayerController());
            if (PC)
            {
                PC->Client_AddChatOnLog(_Chat, _ChatterName);
            }
        }
    }
}


void ASessionGameMode::OnPostLoadMap(UWorld* _World)
{
    if (!_World)
    {
        return;
    }

    if (_World->GetGameInstance() != GetGameInstance())
    {
        return;
    }
    InitializeSession();
}

//OnlineSubsystem으로부터 최초의 Session 정보를 가져옴
void ASessionGameMode::InitializeSession()
{
    ASessionGameState* GS = GetGameState<ASessionGameState>();
    auto SessionInterface = GetSessionInterface();

    if (GS && SessionInterface)
    {
        FNamedOnlineSession* Session = SessionInterface->GetNamedSession(NAME_GameSession);
        if (Session)
        {
            FSessionInfo CurrentSessionInfo;
            Session->SessionSettings.Get(FName("SESSION_NAME"), CurrentSessionInfo.SessionName_);
            CurrentSessionInfo.MaxUserCount_ = Session->SessionSettings.NumPrivateConnections;
            CurrentSessionInfo.CurrentUserCount_ = 1;
        	CurrentSessionInfo.bIsPrivate_ = true;

            FString MapName;
            Session->SessionSettings.Get(FName("MAPDATA"), MapName);
            CurrentSessionInfo.SelectedMapData_ = FPrimaryAssetId(MapName);

        	SessionInterface->OnUpdateSessionCompleteDelegates.AddUObject(this, &ASessionGameMode::OnSessionUpdated);
            GS->UpdateSession(CurrentSessionInfo);
            GS->SetHost(Session->OwningUserId);
        }
    }
}

void ASessionGameMode::OnSessionUpdated(FName _SessionName, bool _bWasSuccessful)
{
	if (false == _bWasSuccessful)
	{
		return;
	}
	
	ASessionGameState* GS = GetGameState<ASessionGameState>();
	auto SessionInterface = GetSessionInterface();

	if (GS && SessionInterface)
	{
		FNamedOnlineSession* Session = SessionInterface->GetNamedSession(_SessionName);
		if (Session)
		{
			FSessionInfo SessionInfo;
			Session->SessionSettings.Get(FName("SESSION_NAME"), SessionInfo.SessionName_);
			SessionInfo.bIsPrivate_ = (0 < Session->SessionSettings.NumPrivateConnections)
												&& (0 == Session->SessionSettings.NumPublicConnections);
			
			if (SessionInfo.bIsPrivate_)
			{
				SessionInfo.MaxUserCount_ = Session->SessionSettings.NumPrivateConnections;
			}
			else
			{
				SessionInfo.MaxUserCount_ = Session->SessionSettings.NumPublicConnections;
			}
			
			FString MapName;
			Session->SessionSettings.Get(FName("MAPDATA"), MapName);
			SessionInfo.SelectedMapData_ = FPrimaryAssetId(MapName);
			GS->UpdateSession(SessionInfo);
		}
	}
}

TSharedPtr<IOnlineSession, ESPMode::ThreadSafe> ASessionGameMode::GetSessionInterface()
{
    IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
    if (Subsystem)
    {
        return Subsystem->GetSessionInterface();
    }
    return nullptr;
}
