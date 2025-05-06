#include "Skill.h"
#include "ICondition.h"

USkill::USkill()
{
	SKillName_ = L"Default Skill";
	RemainingUses_ = 0;
}


bool USkill::CheckConditions() const
{
	//TODO : 어떤 사유로 스킬을 사용할 수 없음을 안내
	if(RemainingUses_ == 0)
	{
		return false;
	}

	for(const TSharedPtr<IICondition>& Condition : Conditions_)
	{
		if(false == Condition->Check())
		{
			return false;
		}
	}
	return true;
}


