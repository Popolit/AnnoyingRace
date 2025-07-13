#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitBox.generated.h"

UCLASS(Abstract, HideDropdown)
class ANNOYINGRACE_API AHitBox : public AActor
{
	GENERATED_BODY()

public:
	AHitBox();

	virtual void BeginPlay() override;

	void EnableCollisions();

	UFUNCTION()
		void OnCollisionOverlapped(UPrimitiveComponent* _OverlappedComponent,
		AActor* _OtherActor,
		UPrimitiveComponent* _OtherComp,
		int32 _OtherBodyIndex,
		bool _bFromSweep,
		const FHitResult& _SweepResult);
};
