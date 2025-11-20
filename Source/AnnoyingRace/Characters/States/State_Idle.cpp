#include "State_Idle.h"

#include "InputAction.h"
#include "Characters/PlayableCharacter.h"

void UState_Idle::Enter(class UStateComponent* _Comp)
{
}

void UState_Idle::Exit(class UStateComponent* _Comp)
{
}

void UState_Idle::Move(const FInputActionInstance& _Instance, ACharacter* _Character)
{
	if (_Character)
	{
		const FVector2D MovingFactor = _Instance.GetValue().Get<FVector2D>();
		FVector Direction = FVector(MovingFactor.X, MovingFactor.Y, 0.0f);

		const FRotator CharacterRotation = _Character->GetControlRotation();
		Direction = CharacterRotation.RotateVector(Direction);

		_Character->AddMovementInput(Direction.GetSafeNormal2D());
	}
}

void UState_Idle::Look(const FInputActionInstance& _Instance, ACharacter* _Character)
{
	if (_Character)
	{
		const FVector2D LookingFactor = _Instance.GetValue().Get<FVector2D>();
		_Character->AddControllerYawInput(LookingFactor.X);
		_Character->AddControllerPitchInput(LookingFactor.Y);
	}
}

void UState_Idle::SkillButtonPushed(const FInputActionInstance& _Instance, ACharacter* _Character)
{
	APlayableCharacter* PlayableCharacter = Cast<APlayableCharacter>(_Character);
	if(PlayableCharacter)
	{
		PlayableCharacter->SkillButtonPushed();
	}
}

void UState_Idle::CancelSkillButtonPushed(const FInputActionInstance& _Instance, ACharacter* _Character)
{
	APlayableCharacter* PlayableCharacter = Cast<APlayableCharacter>(_Character);
	if(PlayableCharacter)
	{
		PlayableCharacter->CancelSkillButtonPushed();
	}
}

void UState_Idle::TakeDamage(ACharacter* _Character, float _DamageAmount, FDamageEvent const& _DamageEvent, AController* _EventInstigator,
                             AActor* _DamageCauser)
{
	APlayableCharacter* PC = Cast<APlayableCharacter>(_Character);

	if (ensureMsgf(PC, TEXT("Character was nullptr")))
	{
		PC->ProcessHit(_DamageAmount, _DamageEvent);
	}
}

