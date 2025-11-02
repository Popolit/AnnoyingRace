#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RaceGameState.generated.h"

DECLARE_DELEGATE(FOnPlayerRankingUpdated)

/**
 * Race Game State
 */
UCLASS()
class ANNOYINGRACE_API ARaceGameState : public AGameState
{
	GENERATED_BODY()
public:
	ARaceGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void HandleMatchHasStarted() override;

public:
	void HandleStartRace();

	void SetMaxCheckPointCount(const uint8 _Count);

	uint8 GetMaxCheckPointCount() const;

	void SetMaxLap(const uint8 _MaxLaps);

	uint8 GetMaxLap() const;

	float GetRaceElapsedTime() const;

	const TArray<TObjectPtr<APlayerState>>& GetPlayerRankings() const;

private:
	void UpdatePlayerRankings();

	UFUNCTION()
		void OnRep_PlayerRankings();


public:
	FOnPlayerRankingUpdated OnPlayerRankingUpdated_;

private:
	uint8 MaxLap_;

	uint8 MaxCheckPointCount_;

	UPROPERTY(Replicated)
		float RaceStartTime_;

	long double ElapsedTime_;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerRankings)
		TArray<TObjectPtr<APlayerState>> PlayerRankings_;

	FTimerHandle RankingUpdateTimerHandle;
};
