#pragma once

#include "CoreMinimal.h"
#include "Trigger.h"
#include "UObject/NoExportTypes.h"
#include "Trigger_SkillButtonPushed.generated.h"


/**
 * When Skill Button was Pushed
 */
UCLASS()
class ANNOYINGRACE_API UTrigger_SkillButtonPushed : public UTrigger
{
	GENERATED_BODY()

public:
	virtual void Initialize(ACharacter* _Character, USkill* _OwnerSkill) override;
};
