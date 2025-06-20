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

private:
	void StartRaceCountDown();

	void RemoveBlockVolumes();

	void ShuffleCharacterQueue();

	void ChangeCharacter();

private:
	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<class APlayableCharacter>> CharacterPool_;

	TQueue<TSubclassOf<APlayableCharacter>> CharacterQueue_;
};
