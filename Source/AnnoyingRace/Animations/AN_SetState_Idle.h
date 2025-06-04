#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SetState_Idle.generated.h"

/**
 * Set State to Idle
 */
UCLASS()
class ANNOYINGRACE_API UAN_SetState_Idle : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* _MeshComp, UAnimSequenceBase* _Animation) override;
};
