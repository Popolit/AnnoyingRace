#pragma once

#include "CoreMinimal.h"
#include "Skills/Skill.h"
#include "Skill_SpawnChicken.generated.h"

/**
 * Skill of Chicken : Triggered It Dead. -> Spawn Chicken
 */
UCLASS(Blueprintable, HideDropdown)
class ANNOYINGRACE_API USkill_SpawnChicken : public USkill
{
	GENERATED_BODY()

public:
	virtual void Initialize(ACharacter* _Character) override;

	virtual void TriggerSkill(ACharacter* _Character) override;

protected:
	UPROPERTY(EditDefaultsOnly)
		TSoftClassPtr<class ARoastedChicken> RoastedChickenClass_;
};
