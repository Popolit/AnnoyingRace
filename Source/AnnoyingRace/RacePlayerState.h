#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RacePlayerState.generated.h"

DECLARE_DELEGATE_OneParam(FOnLapsChanged, uint8)

/**
 * Race Player State
 */
UCLASS()
class ANNOYINGRACE_API ARacePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ARacePlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	TObjectPtr<class UCharacterData> GetCharacterData() const;

	void SetCharacterData(UCharacterData* _CharacterData);

	uint8 GetLaps() const;

	void IncreaseLap();

	uint8 GetCheckPointIndex() const;

	void SetCheckPointIndex(uint8 _Index);

	float GetTotalDistance() const;

	void SetTotalDistance(float _Distance);

private:
	UFUNCTION()
		void OnRep_Laps();

private:
	TObjectPtr<class UCharacterData> CharacterData_;

	UPROPERTY(ReplicatedUsing = OnRep_Laps)
		uint8 Laps_;

	uint8 CurrentCheckPointIndex_;

	float Distance_;

public:
	FOnLapsChanged OnLapsChanged_;
};
