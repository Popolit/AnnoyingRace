#include "Trigger_SkillButtonPushed.h"

#include "Characters/PlayableCharacter.h"
#include "GameFramework/Character.h"
#include "Skills/Skill.h"


void UTrigger_SkillButtonPushed::Initialize(ACharacter* _Character, USkill* _OwnerSkill)
{
	APlayableCharacter* PlayableCharacter = Cast<APlayableCharacter>(_Character);
	PlayableCharacter->OnSkillButtonPushed_.BindUObject(_OwnerSkill, &USkill::TryTriggerSkill);
}
