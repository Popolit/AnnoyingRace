#pragma once

#include "CoreMinimal.h"
#include "RaceGameResultData.generated.h"

USTRUCT()
struct FRaceGameResultData
{
	GENERATED_USTRUCT_BODY()
public:
	FRaceGameResultData() : PlayerName_(), PlayerRank_(0) {}
	FRaceGameResultData(FString _PlayerName, int32 _PlayerRank) :
		PlayerName_(_PlayerName),
		PlayerRank_(_PlayerRank) {}

public:
	UPROPERTY()
		FString PlayerName_;

	UPROPERTY()
		int32 PlayerRank_;
};
