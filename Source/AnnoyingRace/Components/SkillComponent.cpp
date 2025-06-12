#include "Components/SkillComponent.h"

#include "Skills/Skill.h"
#include "GameFramework/Character.h"


void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Character = Cast<ACharacter>(GetOuter());
	check(Character);

	check(SkillClass_);
	Skill_ = NewObject<USkill>(this, SkillClass_);
	Skill_->Initialize(Character);
}

void USkillComponent::SetTrigger(TFunction<void(ACharacter*)> _Func)
{
	TriggerFunc_ = _Func;
}

uint8 USkillComponent::GetSkillDamage() const
{
	return Skill_->GetDamage();
}

void USkillComponent:: SkillButtonPushed(ACharacter* _Character)
{
	if(TriggerFunc_)
	{
		TriggerFunc_(_Character);
	}
}
