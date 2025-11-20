#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SwapPositionSkill.generated.h"

/**
 * Swap Position Skill
 */
UCLASS()
class ANNOYINGRACE_API UAN_SwapPositionSkill : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* _MeshComp, UAnimSequenceBase* _Animation, const FAnimNotifyEventReference& _EventReference) override;

private:
	void SwapPosition(APawn* _OwnerPawn, APawn* _TargetPawn);
};
