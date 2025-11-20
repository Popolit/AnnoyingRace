#pragma once

#include "CoreMinimal.h"
#include "TargetingMechanism.h"
#include "TM_OneNearMouse.generated.h"

/**
 * Targeting One Actor Near Mouse
 */
UCLASS(Blueprintable)
class ANNOYINGRACE_API UTM_OneNearMouse : public UTargetingMechanism
{
	GENERATED_BODY()

public:
	virtual void SetTargets(ACharacter* _Character) override;

private:
	UPROPERTY(EditAnywhere)
		float Range_ = 5000000.f;

	//타게팅 시, 마우스 주변의 어느 정도 범위를 탐지할 것인지 
	UPROPERTY(EditAnywhere)
		float AimRadius_ = 10.f;
};
