#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaceFinishedWidget.generated.h"

/**
 * Race Finished Widget
 */
UCLASS()
class ANNOYINGRACE_API URaceFinishedWidget : public UUserWidget
{
	GENERATED_BODY()
protected:	
	virtual void NativeConstruct() override;

public:
	void SetRank(const int32 _Rank);

private:
	UFUNCTION(BlueprintCallable)
		void UpdateCountDown();
	
private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UWidgetSwitcher> WST_Rank_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> Txt_CountDown_;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* Anim_CountDown_;

private:
	int32 Count_;
};
