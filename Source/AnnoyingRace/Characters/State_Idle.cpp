#include "Characters/State_Idle.h"

#include "InputAction.h"
#include "GameFramework/Character.h"

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

void UState_Idle::UseSkill(const FInputActionInstance& _Instance, ACharacter* _Character)
{
	
}

