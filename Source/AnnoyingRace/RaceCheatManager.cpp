#include "RaceCheatManager.h"

#include "RaceGameMode.h"
#include "Kismet/GameplayStatics.h"

void URaceCheatManager::FinishRace()
{
	ARaceGameMode* GM = Cast<ARaceGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->FinishRace();
	}
}
