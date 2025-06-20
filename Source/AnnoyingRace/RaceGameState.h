#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RaceGameState.generated.h"

/**
 * Race Game State
 */
UCLASS()
class ANNOYINGRACE_API ARaceGameState : public AGameStateBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
		float ElapsedTime_;
};
