#include "Skills/Condition_IdleState.h"
#include "Components/StateComponent.h"

bool UCondition_IdleState::CheckCondition(UObject* _Target) const
{
	AActor* Actor = Cast<AActor>(_Target);

	if(Actor)
	{
		UStateComponent* StateComponent = Cast<UStateComponent>(Actor->GetComponentByClass(UStateComponent::StaticClass()));

		if(StateComponent)
		{
			return StateComponent->CheckCurrentState(EState::Idle);
		}
	}
	return false;
}
