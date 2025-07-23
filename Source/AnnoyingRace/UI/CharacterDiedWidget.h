#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterDiedWidget.generated.h"

DECLARE_DELEGATE(FOnCharacterDIedAnimationFinished)

/**
 * Show UI When Character Died
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API UCharacterDiedWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetWidget();

	void ShowWidget();

protected:
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* _Animation) override;

private:
	UFUNCTION(BlueprintCallable)
		void UpdateCountDown();

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> Txt_KilledUserName_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> Txt_CountDown_;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		TObjectPtr<UWidgetAnimation> Anim_CountDown_;

private:
	uint8 Count_;

public:
	FOnCharacterDIedAnimationFinished OnCharacterDIedAnimationFinished_;
};