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

void ASessionGameMode::ChangeSessionName(const FString& _SessionName)
{
    ASessionGameState* GS = GetGameState<ASessionGameState>();
    auto SessionInterface = GetSessionInterface();

    if (GS && SessionInterface)
    {
        FNamedOnlineSession* Session = SessionInterface->GetNamedSession(NAME_GameSession);
        if (Session)
        {
            Session->SessionSettings.Set(FName("SESSION_NAME"), _SessionName);
            GS->ChangeSessionName(_SessionName);
            SessionInterface->UpdateSession(NAME_GameSession, Session->SessionSettings);
        }
    }
}

void ASessionGameMode::ChangeSessionPassword(const FString& _SessionPassword)
{
    ASessionGameState* GS = GetGameState<ASessionGameState>();
    auto SessionInterface = GetSessionInterface();

    if (GS && SessionInterface)
    {
        FNamedOnlineSession* Session = SessionInterface->GetNamedSession(NAME_GameSession);
        if (Session)
        {
            Session->SessionSettings.Set(FName("PASSWORD"), _SessionPassword);
            GS->ChangeSessionPassword(_SessionPassword);
            SessionInterface->UpdateSession(NAME_GameSession, Session->SessionSettings);
        }
    }
}

void ASessionGameMode::ChangeSessionIsPrivate(bool _bIsPrivate, const FString& _SessionPassword)
{
    ASessionGameState* GS = GetGameState<ASessionGameState>();
    auto SessionInterface = GetSessionInterface();

    if (GS && SessionInterface)
    {
        FNamedOnlineSession* Session = SessionInterface->GetNamedSession(NAME_GameSession);
        if (Session)
        {
            GS->ChangeSessionIsPrivate(_bIsPrivate);
            if (_bIsPrivate)
            {
                Session->SessionSettings.Set(FName("PASSWORD"), _SessionPassword);
                GS->ChangeSessionPassword(_SessionPassword);
            }
            else
            {
                Session->SessionSettings.Remove(FName("PASSWORD"));
            }
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
            CurrentSessionInfo.MaxUserCount_ = Session->SessionSettings.NumPublicConnections;
            CurrentSessionInfo.CurrentUserCount_ = 1;
            Session->SessionSettings.Get(FName("PASSWORD"), CurrentSessionInfo.Password_);
            CurrentSessionInfo.bIsPrivate_ = !CurrentSessionInfo.Password_.IsEmpty();

            FString MapName;
            Session->SessionSettings.Get(FName("MAPDATA"), MapName);
            CurrentSessionInfo.SelectedMapData_ = FPrimaryAssetId(MapName);
            
            GS->InitializeSession(CurrentSessionInfo);
            GS->SetHost(Session->OwningUserId);
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
