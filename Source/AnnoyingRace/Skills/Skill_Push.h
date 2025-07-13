#pragma once

#include "CoreMinimal.h"
#include "Skill_NonTarget.h"
#include "Skill_Push.generated.h"

/**
 * Push everything in front of Character 
 */
UCLASS(Blueprintable, HideDropdown)
class ANNOYINGRACE_API USkill_Push : public USkill_NonTarget
{
	GENERATED_BODY()

public:
	virtual void Initialize(ACharacter* _Character) override;

	virtual void TriggerSkill(ACharacter* _Character) override;


};
