#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 * Player UI
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateUserRank() const;

	void UpdateUserLap(uint8 _Laps) const;

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> Txt_UserRank_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> Txt_UserLap_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UCanvasPanel> CP_LeaderBoard_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class USkillWidget> SkillWidget_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class URaceRankSlotWidget> SlotWidgetClass_;

	TMap<TObjectKey<APlayerState>, TObjectPtr<URaceRankSlotWidget>> PlayerSlots_;

private:
	uint8 MaxLap_;
};
