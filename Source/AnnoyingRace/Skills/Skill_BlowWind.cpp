#include "Skill_BlowWind.h"

#include "Conditions/Condition_IdleState.h"
#include "GameFramework/Character.h"

void USkill_BlowWind::Initialize(ACharacter* _Character)
{
	Super::Initialize(_Character);

	Conditions_.Add(NewObject<UCondition_IdleState>());
}

void USkill_BlowWind::TriggerSkill(ACharacter* _Character)
{
	PlayAnimMontage(_Character);

	FVector LaunchVector = _Character->GetActorForwardVector();
	_Character->LaunchCharacter(LaunchVector * -KnockbackAmount_, true, false);
}
