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
	void DrawNewCharacter(uint8 _CheckPointIndex, APlayerController* _PC);

	//체크포인트 달성시 Laps 증가 및 캐릭터 드로우
	void HandleCheckPointPassed(uint8 _CheckPointIndex, APlayerController* _PC);

private:
	TSubclassOf<ACharacter> PopNextCharacter();

	void StartRaceCountDown();

	void RemoveBlockVolumes();

	void ShuffleCharacterQueue();

	void ChangeCharacter();

private:
	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<ACharacter>> CharacterPool_;

	TQueue<TSubclassOf<ACharacter>> CharacterQueue_;

	TObjectPtr<class ATrackSplineActor> TrackSpline_;
};
