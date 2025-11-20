#include "AN_SwapPositionSkill.h"

#include "RacePlayerState.h"
#include "Components/SkillComponent.h"
#include "Components/StateComponent.h"

class UStateComponent;

void UAN_SwapPositionSkill::Notify(USkeletalMeshComponent* _MeshComp, UAnimSequenceBase* _Animation,
                                   const FAnimNotifyEventReference& _EventReference)
{
	check(_MeshComp);
	
	Super::Notify(_MeshComp , _Animation , _EventReference);

	//Editor일 경우
	if (false == _MeshComp->HasBegunPlay())
	{
		return;
	}

	APawn* OwnerPawn = Cast<APawn>(_MeshComp->GetOwner());
	if(OwnerPawn && OwnerPawn->HasAuthority())
	{
		auto SkillComp = OwnerPawn->GetComponentByClass<USkillComponent>();

		if (nullptr == SkillComp)
		{
			return;
		}

		TArray<AActor*> TargetActors;
		SkillComp->GetSkillTargets(TargetActors);

		APawn* TargetPawn = nullptr;
		if (false == TargetActors.IsEmpty())
		{
			TargetPawn = Cast<APawn>(TargetActors.Top());
		}
		if (nullptr == TargetPawn)
		{
			return;
		}

		SwapPosition(OwnerPawn, TargetPawn);
	}
}

//위치, 레이스 진행 상황 스왑
void UAN_SwapPositionSkill::SwapPosition(APawn* _OwnerPawn, APawn* _TargetPawn)
{
	auto OwnerState = _OwnerPawn->GetController()->GetPlayerState<ARacePlayerState>();
	auto TargetState = _TargetPawn->GetController()->GetPlayerState<ARacePlayerState>();

	if (OwnerState && TargetState)
	{
		FVector3d PlayerPosition = _OwnerPawn->GetActorLocation();
		FVector3d TargetPosition = _TargetPawn->GetActorLocation();

		_OwnerPawn->SetActorLocation(TargetPosition);
		_TargetPawn->SetActorLocation(PlayerPosition);

		const AActor* OwnerCheckPoint = OwnerState->GetPassedCheckPoint();
		const AActor* TargetCheckPoint = TargetState->GetPassedCheckPoint();
		
		OwnerState->SetCheckPoint(TargetCheckPoint);
		TargetState->SetCheckPoint(OwnerCheckPoint);

		auto StateComp = _TargetPawn->GetComponentByClass<UStateComponent>();
		if (StateComp)
		{
			StateComp->SetState(EState::Idle);
		}
	}
}
