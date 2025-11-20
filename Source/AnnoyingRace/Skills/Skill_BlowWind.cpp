#include "Skill_BlowWind.h"

#include "GameFramework/Character.h"


void USkill_BlowWind::TriggerSkill(ACharacter* _Character)
{
	Super::TriggerSkill(_Character);
	
	PlayAnimMontage(_Character);
	FVector LaunchVector = _Character->GetActorForwardVector();
	_Character->LaunchCharacter(LaunchVector * -SelfKnockbackAmount_, true, false);
}
