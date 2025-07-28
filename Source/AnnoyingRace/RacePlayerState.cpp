#include "RacePlayerState.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ARacePlayerState::ARacePlayerState()
{
	Laps_ = 1;
	CurrentCheckPointIndex_ = 0;
	Distance_ = 0;
}


void ARacePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARacePlayerState, Laps_);
	DOREPLIFETIME(ARacePlayerState, CharacterData_);
}

TObjectPtr<UCharacterData> ARacePlayerState::GetCharacterData() const
{
	return CharacterData_;
}

void ARacePlayerState::SetCharacterData(UCharacterData* _CharacterData)
{
	CharacterData_ = _CharacterData;
}

uint8 ARacePlayerState::GetLaps() const
{
	return Laps_;
}

void ARacePlayerState::IncreaseLap()
{
	if(HasAuthority())
	{
		Laps_++;
		OnRep_Laps();
	}
}

uint8 ARacePlayerState::GetCheckPointIndex() const
{
	return CurrentCheckPointIndex_;
}

void ARacePlayerState::SetCheckPointIndex(uint8 _Index)
{
	CurrentCheckPointIndex_ = _Index;
}

float ARacePlayerState::GetTotalDistance() const
{
	return Distance_;
}

void ARacePlayerState::SetTotalDistance(float _Distance)
{
	Distance_ = _Distance;
}

void ARacePlayerState::OnRep_Laps()
{
	OnLapsChanged_.ExecuteIfBound(Laps_);
}
