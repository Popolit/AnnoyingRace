#include "Skill.h"

#include "Characters/PlayableCharacter.h"
#include "Conditions/Condition.h"
#include "Net/UnrealNetwork.h"
#include "Triggers/Trigger.h"

USkill::USkill()
{
	SKillName_ = L"Default Skill";
	RemainingUses_ = 0;
	Damage_ = 0;
}

void USkill::Tick(float _DeltaTime)
{
	if (0.f < SkillCoolDownLeft_)
	{
		SkillCoolDownLeft_ -= _DeltaTime;
		if (SkillCoolDownLeft_ < 0.f)
		{
			SkillCoolDownLeft_ = 0.f;
		}
	}
}

bool USkill::IsSupportedForNetworking() const
{
	return true;
}

void USkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USkill, RemainingUses_);
}

void USkill::Initialize(ACharacter* _Character)
{
	if (SkillTrigger_)
	{
		SkillTrigger_->Initialize(_Character, this);
	}
}

uint8 USkill::GetSkillDamage() const
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

TSoftObjectPtr<UTexture2D> USkill::GetSkillImg()
{
	return SkillImg_;
}

int32 USkill::GetSkillCount() const
{
	return RemainingUses_;
}

//쿨타임 0인 스킬의 경우 0
float USkill::GetSkillCoolDownRatio() const
{
	if (SkillCoolDown_ == 0.f)
	{
		return 0.f;
	}
	return SkillCoolDownLeft_ / SkillCoolDown_;
}

bool USkill::CheckConditions(ACharacter* _Character) const
{
	//TODO : 어떤 이유로 스킬을 사용할 수 없는지 안내
	if(RemainingUses_ == 0 || 0.f < SkillCoolDownLeft_)
	{
		return false;
	}
	if (SkillCondition_ && false == SkillCondition_->CheckCondition(_Character))
	{
		return false;
	}

	return true;
}

void USkill::TriggerSkill(ACharacter* _Character)
{
	if (0 < RemainingUses_)
	{
		RemainingUses_--;
	}
	SkillCoolDownLeft_ = SkillCoolDown_;
}

void USkill::PlayAnimMontage(ACharacter* _Character) const
{
	APlayableCharacter* Character = Cast<APlayableCharacter>(_Character);
	if(Character)
	{
		Character->Server_PlayAnimMontage(Animation_);
	}
}

