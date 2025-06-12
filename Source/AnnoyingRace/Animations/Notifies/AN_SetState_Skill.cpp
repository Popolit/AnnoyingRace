#include "AN_SetState_Skill.h"

#include "Components/StateComponent.h"

void UAN_SetState_Skill::Notify(USkeletalMeshComponent* _MeshComp, UAnimSequenceBase* _Animation)
{
	AActor* OwnerActor = _MeshComp->GetOwner();

	if (ensureMsgf(OwnerActor, TEXT("%s's OwnerActor was nullptr"), *this->GetName()))
	{
		//Editor PreviewÀÎ °æ¿ì
		if (false == OwnerActor->HasActorBegunPlay())
		{
			return;
		}

		UStateComponent* StateComponent = Cast<UStateComponent>(OwnerActor->GetComponentByClass(UStateComponent::StaticClass()));

		if (ensureMsgf(StateComponent, TEXT("%s's StateComponent was nullptr"), *OwnerActor->GetName()))
		{
			StateComponent->SetState(EState::Skill);
		}
	}
}
