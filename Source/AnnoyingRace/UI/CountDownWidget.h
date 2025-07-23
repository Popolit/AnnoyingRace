#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountDownWidget.generated.h"

DECLARE_DELEGATE(FOnCountdownFinished);

/**
 * Show Count Down when Race Begin
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API UCountDownWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* _Animation) override;

	UFUNCTION(BlueprintCallable)
		void UpdateCountDown();

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> Txt_CountDown_;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		TObjectPtr<UWidgetAnimation> Anim_CountDown_;

	UINT CountDown_ = 3;

public:
	FOnCountdownFinished OnCountdownAnimationFinished_;
};
