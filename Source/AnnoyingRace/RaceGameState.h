#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RaceGameState.generated.h"

/**
 * Race Game State
 */
UCLASS()
class ANNOYINGRACE_API ARaceGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	ARaceGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetMaxCheckPointCount(uint8 _Count);

	uint8 GetMaxCheckPointCount() const;

	void SetMaxLap(uint8 _MaxLaps);

	uint8 GetMaxLap() const;

public:
	UPROPERTY(ReplicatedUsing = OnRep_CountdownStartTime)
		float CountdownStartTime_ = 0.f;

	UFUNCTION()
		void OnRep_CountdownStartTime();


private:
	uint8 MaxLap_;
	uint8 MaxCheckPointCount_;
	float ElapsedTime_;
};
