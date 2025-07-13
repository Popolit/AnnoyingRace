#pragma once

#include "CoreMinimal.h"
#include "Skills/Triggers/Trigger.h"
#include "Trigger_Death.generated.h"

/**
 * Triggered When Character Death
 */
UCLASS()
class ANNOYINGRACE_API UTrigger_Death : public UTrigger
{
	GENERATED_BODY()

public:
	virtual void Initialize(ACharacter* _Character, USkill* _OwnerSkill) override;
};
