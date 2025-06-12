#include "Skill.h"

#include "Conditions/ICondition.h"
#include "Triggers/ITrigger.h"
#include "GameFramework/Character.h"

USkill::USkill()
{
	SKillName_ = L"Default Skill";
	RemainingUses_ = 0;
	Damage_ = 0;
}

void USkill::Initialize(ACharacter* _Character)
{
	IITrigger* Trigger = SkillTrigger_.GetInterface();
	if (Trigger)
	{
		Trigger->Bind(_Character, [this](ACharacter* _Character)
			{ this->TryTriggerSkill(_Character); });
	}
}

uint8 USkill::GetDamage() const
{
	return Damage_;
}

void USkill::TryTriggerSkill(ACharacter* _Character)
{
	if(CheckConditions(_Character))
	{
		TriggerSkill(_Character);
	}
}

bool USkill::CheckConditions(ACharacter* _Character) const
{
	//TODO : 어떤 사유로 스킬을 사용할 수 없음을 안내
	if(RemainingUses_ == 0)
	{
		return false;
	}

	for(const TObjectPtr<IICondition>& Condition : Conditions_)
	{
		if(false == Condition->CheckCondition(Cast<UObject>(_Character)))
		{
			return false;
		}
	}
	return true;
}


