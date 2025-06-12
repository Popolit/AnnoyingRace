#include "State_Skill.h"

#include "Characters/PlayableCharacter.h"


void UState_Skill::Enter(class UStateComponent* _Comp)
{
}

void UState_Skill::Exit(class UStateComponent* _Comp)
{
}

void UState_Skill::Move(const FInputActionInstance& _Instance, ACharacter* _Character)
{
}

void UState_Skill::Look(const FInputActionInstance& _Instance, ACharacter* _Character)
{
}

void UState_Skill::SkillButtonPushed(const FInputActionInstance& _Instance, ACharacter* _Character)
{
}

void UState_Skill::TakeDamage(ACharacter* _Character, float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator,
	AActor* _DamageCauser)
{
	APlayableCharacter* PC = Cast<APlayableCharacter>(_Character);

	if (ensureMsgf(PC, TEXT("Character was nullptr")))
	{
		PC->ProcessHit(_DamageAmount, _DamageEvent);
	}
}
