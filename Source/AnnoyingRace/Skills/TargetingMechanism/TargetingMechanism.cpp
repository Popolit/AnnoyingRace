#include "TargetingMechanism.h"

void UTargetingMechanism::GetTargets(TArray<AActor*>& _OutTargets)
{
	_OutTargets = TargetActors_;
}
