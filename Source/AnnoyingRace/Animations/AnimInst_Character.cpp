#include "AnimInst_Character.h"

void UAnimInst_Character::NativeUpdateAnimation(float _DeltaSeconds)
{
	const APawn* Character = TryGetPawnOwner();
	if(Character)
	{
		Speed_ = Character->GetVelocity().Size2D();
	}
}
