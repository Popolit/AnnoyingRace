#include "Action_Move.h"
#include "GameFramework/Character.h"

void UAction_Move::DoAction()
{
	ACharacter* Character = Cast<ACharacter>(GetOuter());

	if(ensureMsgf(Character, TEXT("%s's Owner Character was nullptr"), *GetName()))
	{
	}
}
