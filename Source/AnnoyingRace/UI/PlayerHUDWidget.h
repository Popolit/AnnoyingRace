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
	void UpdateUserRank();

	void UpdateUserLap(uint8 _Laps);

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> Txt_UserRank_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> Txt_UserLap_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UVerticalBox> VB_PlayerList_;

private:
	uint8 MaxLap_;
};
