#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInst_Character.generated.h"

/**
 *	AnimInstance For Character 
 */
UCLASS()
class ANNOYINGRACE_API UAnimInst_Character : public UAnimInstance
{
	GENERATED_BODY()

private:
	virtual void NativeUpdateAnimation(float _DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly)
		float Speed_;
};
