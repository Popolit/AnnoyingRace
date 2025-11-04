#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RaceGameMode.generated.h"

/**
 * Race Game Mode
 */
UCLASS()
class ANNOYINGRACE_API ARaceGameMode : public AGameMode
{
	GENERATED_BODY()
	friend class URaceCheatManager;
public:
	ARaceGameMode();

	virtual void StartPlay() override;

	virtual void StartMatch() override;

	virtual void PostLogin(APlayerController* _NewPlayer) override;

	virtual bool ReadyToStartMatch_Implementation() override;

public:
	void AddReadiedPlayerCount();
	
	void HandlePlayerDeath(APlayerController* _PC) const;

	//Laps등 변화 없이 캐릭터만 드로우
	UFUNCTION(BlueprintCallable)
	void DrawNewCharacter(APlayerController* _PC);

	void SpawnNewCharacter(APlayerController* _PC);

	//체크포인트 달성시 Laps 증가 및 캐릭터 드로우
	void HandleCheckPointPassed(APlayerController* _PC, const class ACheckPointBox* _PassedCheckPoint);

private:
	UFUNCTION(BlueprintCallable)
		void StartRaceCountDown();

	TObjectPtr<class UCharacterData> PopNextCharacter();

	void ShuffleCharacterQueue();

	bool CheckAllPlayersFinishedRace() const;

	void FinishRace();

	void ReturnToSession();
	
private:
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<class UCharacterData>> CharacterPool_;

	TQueue<TObjectPtr<UCharacterData>> CharacterQueue_;

	TObjectPtr<class ATrackSplineActor> TrackSpline_;

	bool bRaceStarted_;

	int32 ReadiedPlayerCount_ = 1;

	FTimerHandle RaceFinishCountDownTimer_;
};
