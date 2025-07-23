
#include "World/TrackSplineActor.h"

#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"


ATrackSplineActor::ATrackSplineActor()
{
	Spline_ = CreateDefaultSubobject<USplineComponent>("Spline");
	SetRootComponent(Spline_);
}

int32 ATrackSplineActor::GetNumberOfCheckPoints() const
{
	return Spline_->GetNumberOfSplinePoints();
}

FTransform ATrackSplineActor::GetSplinePointTransform(int32 _PointIndex) const
{
	check(Spline_);

    const int32 NumberOfPoints = GetNumberOfCheckPoints();
    if (ensureMsgf(0 <= _PointIndex && _PointIndex < NumberOfPoints, TEXT("Invlid Spline Index")))
    {
        FTransform PointTransform = Spline_->GetTransformAtSplinePoint(_PointIndex, ESplineCoordinateSpace::World);

        return PointTransform;
    }

	return FTransform::Identity;
}

float ATrackSplineActor::GetTotalDistance(FVector _Pos) const
{
	check(Spline_);
	
	//현재 가장 가까운 Spline의 지점 가져오기
	const float Distance = Spline_->FindInputKeyClosestToWorldLocation(_Pos);

	return Spline_->GetDistanceAlongSplineAtSplineInputKey(Distance);
}
