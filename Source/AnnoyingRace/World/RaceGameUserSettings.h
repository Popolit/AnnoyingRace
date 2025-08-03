#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "RaceGameUserSettings.generated.h"

/**
 * User's Game Setting
 */
UCLASS(Config=GameUserSettings)
class ANNOYINGRACE_API URaceGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config)
		float MasterVolume = 1.0f;

	UPROPERTY(Config)
		float BGMVolume = 1.0f;

	UPROPERTY(Config)
		float SFXVolume = 1.0f;

	UPROPERTY(Config)
		FString Language_ = "en";
};
