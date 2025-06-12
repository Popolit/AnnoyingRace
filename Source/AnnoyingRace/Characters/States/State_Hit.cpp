#include "State_Hit.h"

#include "InputAction.h"
#include "Characters/PlayableCharacter.h"

void UState_Hit::Enter(UStateComponent* _Comp)
{
}

void UState_Hit::Exit(class UStateComponent* _Comp)
{
}

void UState_Hit::Move(const FInputActionInstance& _Instance, ACharacter* _Character)
{
}

void UState_Hit::Look(const FInputActionInstance& _Instance, ACharacter* _Character)
{
	if(_Character)
	{
		const FVector2D LookingFactor = _Instance.GetValue().Get<FVector2D>();
		_Character->AddControllerYawInput(LookingFactor.X);
		_Character->AddControllerPitchInput(LookingFactor.Y);
	}
}

void UState_Hit::SkillButtonPushed(const FInputActionInstance& _Instance, ACharacter* _Character)
{
}

void UState_Hit::TakeDamage(ACharacter* _Character, float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator,
	AActor* _DamageCauser)
{
	APlayableCharacter* PC = Cast<APlayableCharacter>(_Character);

	if(ensureMsgf(PC, TEXT("Character was nullptr")))
	{
		PC->ProcessHit(_DamageAmount, _DamageEvent);
	}
}
