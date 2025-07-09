#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountDownWidget.generated.h"

DECLARE_DELEGATE(FOnCountdownFinished);

/**
 * Count Down when Race Begin
 */
UCLASS()
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
		TObjectPtr<class UTextBlock> TxtCountDown_;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		TObjectPtr<UWidgetAnimation> AnimCountDown_;

	UINT CountDown_ = 3;

	FTimerHandle CountDownTimerHandle_;

public:
	FOnCountdownFinished OnCountdownAnimationFinished_;
};
