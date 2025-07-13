#include "Trigger_Death.h"

#include "Skills/Skill.h"
#include "Characters/PlayableCharacter.h"

void UTrigger_Death::Initialize(ACharacter* _Character, USkill* _OwnerSkill)
{
	APlayableCharacter* PlayableCharacter = Cast<APlayableCharacter>(_Character);
	PlayableCharacter->OnCharacterDied_.BindUObject(_OwnerSkill, &USkill::TryTriggerSkill);
}
