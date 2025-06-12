#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_SpawnHitBox.generated.h"

/**
 * Spawn HitBox
 */
UCLASS()
class ANNOYINGRACE_API UANS_SpawnHitBox : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* _MeshComp, UAnimSequenceBase* _Animation, float _TotalDuration) override;

	virtual void NotifyEnd(USkeletalMeshComponent* _MeshComp, UAnimSequenceBase* _Animation) override;

private:
	UPROPERTY(EditInstanceOnly)
		TSubclassOf<class AHitBox> HitBoxClass_;

	UPROPERTY()
		TObjectPtr<AHitBox> HitBox_;
};
