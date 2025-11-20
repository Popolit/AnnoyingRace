#include "State_Casting.h"

#include "InputAction.h"
#include "Characters/PlayableCharacter.h"

void UState_Casting::Enter(UStateComponent* _Comp)
{
}

void UState_Casting::Exit(class UStateComponent* _Comp)
{
}

void UState_Casting::Move(const FInputActionInstance& _Instance, ACharacter* _Character)
{
}

void UState_Casting::Look(const FInputActionInstance& _Instance, ACharacter* _Character)
{
	if(_Character)
	{
		const FVector2D LookingFactor = _Instance.GetValue().Get<FVector2D>();
		_Character->AddControllerYawInput(LookingFactor.X);
		_Character->AddControllerPitchInput(LookingFactor.Y);
	}
}

void UState_Casting::SkillButtonPushed(const FInputActionInstance& _Instance, ACharacter* _Character)
{
	APlayableCharacter* PlayableCharacter = Cast<APlayableCharacter>(_Character);
	if(PlayableCharacter)
	{
		PlayableCharacter->SkillButtonPushed();
	}
}

void UState_Casting::CancelSkillButtonPushed(const struct FInputActionInstance& _Instance, ACharacter* _Character)
{
	APlayableCharacter* PlayableCharacter = Cast<APlayableCharacter>(_Character);
	if(PlayableCharacter)
	{
		PlayableCharacter->CancelSkillButtonPushed();
	}
}

void UState_Casting::TakeDamage(ACharacter* _Character, float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator,
                                AActor* _DamageCauser)
{
	APlayableCharacter* PC = Cast<APlayableCharacter>(_Character);

	if(ensureMsgf(PC, TEXT("Character was nullptr")))
	{
		PC->ProcessHit(_DamageAmount, _DamageEvent);
	}
}
