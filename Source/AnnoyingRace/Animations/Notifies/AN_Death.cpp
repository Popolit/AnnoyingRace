#include "AN_Death.h"

void UAN_Death::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(MeshComp->GetOwner()->HasActorBegunPlay())
	{
		MeshComp->GetOwner()->Destroy();
	}
}
