#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RaceGameState.generated.h"

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

	void HandleStartRace();

	void SetSequenceActor(class ALevelSequenceActor* _SequenceActor);

	ALevelSequenceActor* GetSequenceActor() const;

	void SetMaxCheckPointCount(uint8 _Count);

	uint8 GetMaxCheckPointCount() const;

	void SetMaxLap(uint8 _MaxLaps);

	uint8 GetMaxLap() const;

	float GetRaceElapsedTime() const;

private:
	UPROPERTY(Replicated)
		TObjectPtr<ALevelSequenceActor> SequencActor_;

	uint8 MaxLap_;

	uint8 MaxCheckPointCount_;

	UPROPERTY(Replicated)
		float RaceStartTime_;

	long double ElapsedTime_;
};
