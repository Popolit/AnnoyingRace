#include "Skill_Push.h"

#include "Conditions/Condition_IdleState.h"
#include "Triggers/Trigger_SkillButtonPushed.h"
#include "GameFramework/Character.h"


void USkill_Push::Initialize(ACharacter* _Character)
{
	SkillTrigger_ = NewObject<UTrigger_SkillButtonPushed>();

	Super::Initialize(_Character);

	Conditions_.Add(NewObject<UCondition_IdleState>());
}

void USkill_Push::TriggerSkill(ACharacter* _Character)
{
	if (Animation_.IsNull())
	{
		ensureMsgf(true, TEXT("%s's AnimMontage was nullptr"), *GetName());
		return;
	}
	_Character->PlayAnimMontage(Animation_);
}
