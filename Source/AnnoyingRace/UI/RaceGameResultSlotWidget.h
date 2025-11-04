#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaceGameResultSlotWidget.generated.h"

/**
 * Race Gaeme Result Slot Widget
 */
UCLASS()
class ANNOYINGRACE_API URaceGameResultSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWidget(int32 _Rank, const FString& _PlayerName) const;

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UWidgetSwitcher> WST_PlayerRank_;
	
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> Txt_PlayerName_;
};
