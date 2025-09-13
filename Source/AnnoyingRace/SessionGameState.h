#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SessionGameState.generated.h"

USTRUCT()
struct FSessionPlayerInfo
{
	GENERATED_BODY()
public:
	FSessionPlayerInfo() :
		UniqueNetId_(),
		PlayerName_(),
		bIsHost_(false),
		bIsReady_(false) {}
	
	bool operator==(const FSessionPlayerInfo& _Rhs) const
	{
		return UniqueNetId_ == _Rhs.UniqueNetId_;
	}

public:
	UPROPERTY()
		FUniqueNetIdRepl UniqueNetId_;

	UPROPERTY()
		FString PlayerName_;

	UPROPERTY()
		bool bIsHost_;

	UPROPERTY()
		bool bIsReady_;
};

USTRUCT()
struct FSessionInfo
{
	GENERATED_BODY()

public:
	FSessionInfo() :
		SessionName_(),
		SelectedMapData_(),
		MaxUserCount_(8),
		CurrentUserCount_(1),
		bIsPrivate_(false),
		Password_() {}
	
	UPROPERTY()
		FString SessionName_;

	UPROPERTY()
		FPrimaryAssetId SelectedMapData_;

	UPROPERTY()
		uint8 MaxUserCount_;

	UPROPERTY()
		uint8 CurrentUserCount_;

	UPROPERTY()
		bool bIsPrivate_;
	
	UPROPERTY()
		FString Password_;
};


DECLARE_DELEGATE_OneParam(FOnPlayerListUpdated, const TArray<FSessionPlayerInfo>&);
DECLARE_DELEGATE_OneParam(FOnSessionInfoUpdated, const FSessionInfo&);

/**
 * Session Game State
 */
UCLASS()
class ANNOYINGRACE_API ASessionGameState : public AGameState
{
	GENERATED_BODY()
public:
	ASessionGameState();
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	void InitializeSession(const FSessionInfo& _SessionInfo);
    
	void AddPlayer(const APlayerState* _PlayerState);
    
	void RemovePlayer(const APlayerState* _PlayerState);
    
	void TogglePlayerIsReady(const APlayerState* _PlayerState);

	void SetHost(const FUniqueNetIdRepl& _HostId);

	void ChangeSessionName(const FString& _SessionName);

	void ChangeSessionPassword(const FString& _SessionPassword);

	void ChangeSessionIsPrivate(bool _bIsPrivate);

	void SetMap(const FPrimaryAssetId& _MapId);

private:
	void CheckAllPlayersReady();
	
	int8 FindPlayerIndex(const FUniqueNetIdRepl& _PlayerId) const;
	
	UFUNCTION()
		void OnRep_SessionInfo() const;

	UFUNCTION()
		void OnRep_PlayerList() const;

private:
	UPROPERTY(ReplicatedUsing = OnRep_SessionInfo)
		FSessionInfo SessionInfo_;
	
	UPROPERTY(ReplicatedUsing = OnRep_PlayerList)
		TArray<FSessionPlayerInfo> PlayerList_;

	UPROPERTY(Replicated)
		FUniqueNetIdRepl HostId_;

public:
	FOnPlayerListUpdated OnPlayerListUpdated_;
	FOnSessionInfoUpdated OnSessionInfoUpdated_;
};
