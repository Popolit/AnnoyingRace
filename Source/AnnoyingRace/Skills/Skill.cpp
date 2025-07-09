#include "Skill.h"

#include "GameFramework/Character.h"
#include "Conditions/Condition_IdleState.h"
#include "Net/UnrealNetwork.h"
#include "Triggers/Trigger.h"

USkill::USkill()
{
	SKillName_ = L"Default Skill";
	RemainingUses_ = 0;
	Damage_ = 0;
}

bool USkill::IsSupportedForNetworking() const
{
	return true;
}

void USkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USkill, RemainingUses_);
	DOREPLIFETIME(USkill, Conditions_);
}

void USkill::Initialize(ACharacter* _Character)
{
	if(SkillTriggerClass_)
	{
		SkillTrigger_ = NewObject<UTrigger>(this, SkillTriggerClass_);
		SkillTrigger_->Initialize(_Character, this);
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

	/*for(const TObjectPtr<UCondition_IdleState>& Condition : Conditions_)
	{
		if(false == Condition->CheckCondition(Cast<UObject>(_Character)))
		{
			return false;
		}
	}*/
	return true;
}


