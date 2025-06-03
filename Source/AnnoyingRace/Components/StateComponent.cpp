#include "StateComponent.h"

#include "Characters/State_Idle.h"
#include "Characters/State_Hit.h"
#include "Characters/State_Skill.h"

#include "InputAction.h"
#include "GameFramework/Character.h"

UStateComponent::UStateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UStateComponent::BeginPlay()
{
	Super::BeginPlay();

	States_.Add(EState::Idle, NewObject<UState_Idle>(this, "State_Idle"));
	States_.Add(EState::Hit, NewObject<UState_Hit>(this, "State_Hit"));
	States_.Add(EState::Skill, NewObject<UState_Skill>(this, "State_Skill"));

	CurrentState_ = States_[EState::Idle];
}

bool UStateComponent::CheckCurrentState(EState _State)
{
	return CurrentState_ == States_[_State];
}


void UStateComponent::SetState(EState _NewState)
{
	if(false == CurrentState_.IsNull())
	{
		CurrentState_->Exit(this);
	}
	CurrentState_ = States_[_NewState];
	CurrentState_->Enter(this);
}

void UStateComponent::Move(const FInputActionInstance& _Instance)
{
	if (false == CurrentState_.IsNull())
	{
		CurrentState_->Move(_Instance, Cast<ACharacter>(GetOuter()));
	}
}

void UStateComponent::Look(const FInputActionInstance& _Instance)
{
	if (false == CurrentState_.IsNull())
	{
		CurrentState_->Look(_Instance, Cast<ACharacter>(GetOuter()));
	}
}

void UStateComponent::SkillButtonPushed(const FInputActionInstance& _Instance)
{
	if (false == CurrentState_.IsNull())
	{
		CurrentState_->SkillButtonPushed(_Instance, Cast<ACharacter>(GetOuter()));
	}
}
