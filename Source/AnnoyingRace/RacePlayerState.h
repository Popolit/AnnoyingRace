#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RacePlayerState.generated.h"

/**
 * Race Player State
 */
UCLASS()
class ANNOYINGRACE_API ARacePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ARacePlayerState();

	uint8 GetLaps() const;

	void IncreaseLap();

	uint8 GetTargetCheckPointIndex() const;

	void SetTargetCheckPointIndex(uint8 _Index);

private:
	uint8 Laps_;
	uint8 TargetCheckPointIndex_;
	float Distance_;
};
