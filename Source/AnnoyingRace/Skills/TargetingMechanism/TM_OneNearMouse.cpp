#include "TM_OneNearMouse.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UTM_OneNearMouse::SetTargets(ACharacter* _Character)
{
	if (nullptr == _Character)
	{
		return;
	}
	APlayerController* PC = Cast<APlayerController>(_Character->GetController());
	if (!PC) return;

	FVector CameraLocation;
	FRotator CameraRotation;
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	//파라미터 세팅
	FVector StartLocation = CameraLocation + (CameraRotation.Vector() * 50.f);
	FVector EndLocation = StartLocation + (CameraRotation.Vector() * Range_);
	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(AimRadius_);
	const TArray<AActor*> ActorsToIgnore = {_Character};
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	FHitResult HitResult;
	
	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		StartLocation,
		EndLocation,
		AimRadius_,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
		);
	
	TargetActors_.Empty();
	if (bHit)
	{
		TargetActors_.Add(HitResult.GetActor());
	}
}
