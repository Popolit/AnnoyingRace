#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "World/RaceGameResultData.h"
#include "RaceGameInstance.generated.h"

DECLARE_DELEGATE_ThreeParams(FOnSessionFindComplete, bool, const FText&, const TArray<class UCommonSession_SearchResult*>&)
DECLARE_DELEGATE_OneParam(FOnSessionError, const FText&)

struct FSessionData
{
	FString SessionName_;
	bool bPrivate_;
	FString Password_;
	uint8 MaxUserCount_;
	uint8 CurrentUserCount_;
	FPrimaryAssetId MapData_;
};

/**
 * Game Instance
 */
UCLASS()
class ANNOYINGRACE_API URaceGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	virtual void Shutdown() override;

	void CreateSession(APlayerController* _PC, const FSessionData& _SessionData);

	void FindSessions(APlayerController* _PC);

	void JoinSessionWithResult(APlayerController* _PC, UCommonSession_SearchResult* _SearchResult);

	void CleanUpSession(APlayerController* _PC);

	FPrimaryAssetId GetSessionMap() const;

	void SetRacePlayerCount(const int32 _RacePlayerCount);

	int32 GetRacePlayerCount() const;

	void SetRaceLaps(const int32 _RaceLaps);

	uint8 GetRaceLaps() const;

	void SetPrevGameResult(const TArray<TObjectPtr<APlayerState>>& _PlayerRankings);

	bool CheckPrevGameWasExist() const;

	TArray<FRaceGameResultData> GetPrevGameResult() const;

	void ClearPrevGameResult();
	
private:
	void OnCreateSessionComplete(const struct FOnlineResultInformation& _Result);

	void OnJoinSessionComplete(const FOnlineResultInformation& _Result);

	void OnFindSessionsComplete(bool _bSuccess, const FText& _ErrorMessage);

	void HandleNetworkFailure(UWorld* _World, UNetDriver* _NetDriver, ENetworkFailure::Type _FailureType, const FString& _ErrorString);
	
private:
	UPROPERTY()
		TObjectPtr<class UCommonSessionSubsystem> SessionSubsystem_;

	UPROPERTY()
		TObjectPtr<class UCommonSession_SearchSessionRequest> SearchSessionRequest_;

	UPROPERTY(EditAnywhere, Category = "Map")
		FPrimaryAssetId SessionMap_;

	UPROPERTY(EditAnywhere, Category = "Map")
		FPrimaryAssetId LobbyMap_;

	UPROPERTY()
		int32 RacePlayerCount_;

	UPROPERTY()
		uint8 RaceLaps_;

	TArray<FRaceGameResultData> PrevRaceGameResult_;

public:
	FOnSessionFindComplete OnSessionFindComplete_;

	//Error Message 전송용 델리게이트
	FOnSessionError OnSessionError_;

	//Error Message 중 Pending 해야하는 (에러 결과로 맵이 변경되는 경우) 메시지
	FText PendingErrorMessage_;
};
