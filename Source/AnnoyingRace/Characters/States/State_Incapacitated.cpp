#include "State_Incapacitated.h"

#include "InputAction.h"
#include "Characters/PlayableCharacter.h"

void UState_Incapacitated::Enter(class UStateComponent* _Comp)
{
}

void UState_Incapacitated::Exit(UStateComponent* _Comp)
{
}

void UState_Incapacitated::Move(const FInputActionInstance& _Instance, ACharacter* _Character)
{
}

void UState_Incapacitated::Look(const FInputActionInstance& _Instance, ACharacter* _Character)
{
	if(_Character)
	{
		const FVector2D LookingFactor = _Instance.GetValue().Get<FVector2D>();
		_Character->AddControllerYawInput(LookingFactor.X);
		_Character->AddControllerPitchInput(LookingFactor.Y);
	}
}

void UState_Incapacitated::SkillButtonPushed(const FInputActionInstance& _Instance, ACharacter* _Character)
{
}

void UState_Incapacitated::TakeDamage(ACharacter* _Character, float _DamageAmount, FDamageEvent const& _DamageEvent,
	AController* _EventInstigator, AActor* _DamageCauser)
{
	APlayableCharacter* PC = Cast<APlayableCharacter>(_Character);

	if (ensureMsgf(PC, TEXT("Character was nullptr")))
	{
		PC->ProcessHit(_DamageAmount, _DamageEvent);
	}
}
