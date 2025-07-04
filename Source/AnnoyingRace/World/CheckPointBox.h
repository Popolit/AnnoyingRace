#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CheckPointBox.generated.h"

/**
 * CheckPoint VolumeBox, Trigger Changing Character
 */
UCLASS()
class ANNOYINGRACE_API ACheckPointBox : public ATriggerBox
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void OnCollisionOverlapped(UPrimitiveComponent* _OverlappedComponent,
			AActor* _OtherActor,
			UPrimitiveComponent* _OtherComp,
			int32 _OtherBodyIndex,
			bool _bFromSweep,
			const FHitResult& _SweepResult);

private:
	UPROPERTY(EditInstanceOnly)
		uint8 Index_;
};
