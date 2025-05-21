#include "Characters/State_Hit.h"

#include "InputAction.h"
#include "GameFramework/Character.h"

void UState_Hit::Enter(class UStateComponent* _Comp)
{
	//Comp->GetOuter() ->HitAnimation();
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

void UState_Hit::UseSkill(const FInputActionInstance& _Instance, ACharacter* _Character)
{
}
