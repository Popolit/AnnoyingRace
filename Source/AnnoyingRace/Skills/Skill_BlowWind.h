#pragma once

#include "CoreMinimal.h"
#include "Skills/Skill_NonTarget.h"
#include "Skill_BlowWind.generated.h"

/**
 * Skill(Fan) - Blow Wind
 */
UCLASS(Blueprintable, HideDropdown)
class ANNOYINGRACE_API USkill_BlowWind : public USkill_NonTarget
{
	GENERATED_BODY()

public:
	virtual void Initialize(ACharacter* _Character) override;

	virtual void TriggerSkill(ACharacter* _Character) override;

private:
	UPROPERTY(EditDefaultsOnly)
		float KnockbackAmount_ = 500.f;
	
	UPROPERTY(EditDefaultsOnly)
		float SelfKnockbackAmount_ = 500.f;
};
