#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrackSplineActor.generated.h"

UCLASS()
class ANNOYINGRACE_API ATrackSplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrackSplineActor();

	float GetTotalDistance(FVector _Pos) const;

private:
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<class USplineComponent> Spline_;
};
