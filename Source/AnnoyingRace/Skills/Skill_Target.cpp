#include "Skill_Target.h"

#include "TargetingMechanism/TargetingMechanism.h"

void USkill_Target::SetTargets(ACharacter* _Character)
{
	if (TargetingMechanism_)
	{
		TargetingMechanism_->SetTargets(_Character);
	}
}


void USkill_Target::GetTargets(TArray<AActor*>& _OutTargets) const
{
	if (TargetingMechanism_)
	{
		TargetingMechanism_->GetTargets(_OutTargets);
	}
}
