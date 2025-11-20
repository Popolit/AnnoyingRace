#include "Condition_State.h"

bool UCondition_State::CheckCondition(UObject* _Target) const
{
	AActor* Actor = Cast<AActor>(_Target);

	if(Actor)
	{
		UStateComponent* StateComponent = Cast<UStateComponent>(Actor->GetComponentByClass(UStateComponent::StaticClass()));

		if(StateComponent)
		{
			return StateComponent->CheckCurrentState(StateToCheck_);
		}
	}
	return false;
}
