#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RacePlayerState.generated.h"

DECLARE_DELEGATE_OneParam(FOnLapsChanged, uint8)
DECLARE_DELEGATE_OneParam(FOnCharacterDataSet, const class UCharacterData*)

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

	TObjectPtr<UCharacterData> GetCharacterData() const;

	void SetCharacterData(UCharacterData* _CharacterData);

	uint8 GetLaps() const;

	void IncreaseLap();

	TObjectPtr<const AActor> GetPassedCheckPoint() const;

	void SetCheckPoint(const AActor* _TargetCheckPoint);

	FTransform GetSpawnTransform() const;
	
	float GetTotalDistance() const;

	void SetTotalDistance(float _Distance);

	void SetRaceFinished();

	bool IsFinished() const;
	
private:
	UFUNCTION()
		void OnRep_CharacterData();

	UFUNCTION()
		void OnRep_Laps();

private:
	UPROPERTY(ReplicatedUsing = OnRep_CharacterData)
		TObjectPtr<UCharacterData> CharacterData_;

	UPROPERTY(ReplicatedUsing = OnRep_Laps)
		uint8 Laps_;

	TObjectPtr<const AActor> PassedCheckPoint_;

	float Distance_;

public:
	FOnCharacterDataSet OnCharacterDataSet_;

	FOnLapsChanged OnLapsChanged_;

private:
	bool bFinished_;
};
