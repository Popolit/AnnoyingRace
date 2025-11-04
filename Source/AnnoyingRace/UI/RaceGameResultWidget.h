#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "World/RaceGameResultData.h"
#include "RaceGameResultWidget.generated.h"

/**
 * Race Game Result Widget
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API URaceGameResultWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
public:
	void SetGameResult(const TArray<FRaceGameResultData>& _PrevRaceGameResult);

private:
	UFUNCTION()
		void OnBackButtonClicked();
	
private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UVerticalBox> VTB_GameResult_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UButton> Btn_Back_;

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class URaceGameResultSlotWidget> SlotWidgetClass_;
};
