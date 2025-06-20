
#include "World/TrackSplineActor.h"

#include "Components/SplineComponent.h"


ATrackSplineActor::ATrackSplineActor()
{
	Spline_ = CreateDefaultSubobject<USplineComponent>("Spline");
	SetRootComponent(Spline_);
}

float ATrackSplineActor::GetTotalDistance(FVector _Pos) const
{
	check(Spline_);

	//현재 가장 가까운 Spline의 지점 가져오기
	const float Distance = Spline_->FindInputKeyClosestToWorldLocation(_Pos);
	return Spline_->GetSplineLength() - Spline_->GetDistanceAlongSplineAtSplineInputKey(Distance);
}
