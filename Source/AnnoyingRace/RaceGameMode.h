#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RaceGameMode.generated.h"

/**
 * Race Game Mode
 */
UCLASS()
class ANNOYINGRACE_API ARaceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARaceGameMode();

	virtual void StartPlay() override;

	virtual void PostLogin(APlayerController* _NewPlayer) override;

	//Laps등 변화 없이 캐릭터만 드로우
	void DrawNewCharacter(APlayerController* _PC);

	void SpawnNewCharacter(APlayerController* _PC);

	//체크포인트 달성시 Laps 증가 및 캐릭터 드로우
	void HandleCheckPointPassed(uint8 _CheckPointIndex, APlayerController* _PC);

	void PossessToNextCharacter(APlayerController* _PC);

private:
	TObjectPtr<class UCharacterData> PopNextCharacter();

	void StartRaceCountDown();

	void RemoveBlockVolumes();

	void ShuffleCharacterQueue();

	void ChangeCharacter();

private:
	UPROPERTY(EditDefaultsOnly)
		TArray<TObjectPtr<class UCharacterData>> CharacterPool_;

	TQueue<TObjectPtr<UCharacterData>> CharacterQueue_;

	TMap<TObjectPtr<APlayerController>, TObjectPtr<UCharacterData>> PlayersCharacterInfo_;

	TObjectPtr<class ATrackSplineActor> TrackSpline_;

	bool bRaceStarted;
};
