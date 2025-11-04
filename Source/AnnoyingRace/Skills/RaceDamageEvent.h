#pragma once

#include "CoreMinimal.h"
#include "Engine/DamageEvents.h"
#include "RaceDamageEvent.generated.h"

USTRUCT()
struct FRaceDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	FVector KnockbackVector_;
};