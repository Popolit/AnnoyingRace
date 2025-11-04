#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "RaceCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class ANNOYINGRACE_API URaceCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Exec)
		void FinishRace();
};
