#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "RaceWorldSettings.generated.h"

/**
 * Race World Settings
 */
UCLASS()
class ANNOYINGRACE_API ARaceWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "Level Config")
        TObjectPtr<class USoundCue> WorldBGM_;
};