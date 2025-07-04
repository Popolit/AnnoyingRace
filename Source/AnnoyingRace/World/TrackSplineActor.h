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

	int32 GetNumberOfCheckPoints() const;

	FTransform GetSplinePointTransform(int32 _PointIndex) const;

	float GetTotalDistance(FVector _Pos) const;

private:
	//구조상 반드시 2개 이상의 체크포인트가 필요함.
	//1개일 경우, 체크포인트에서 나갔다가 들어오는 경우 Laps 증가 처리됨.
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<class USplineComponent> Spline_;
};
