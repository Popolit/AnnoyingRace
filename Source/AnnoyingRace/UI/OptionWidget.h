#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionWidget.generated.h"

/**
 * Game Option UI
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API UOptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOptionWidget(const FObjectInitializer& _ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
		void OnClickedGraphicsBtn();

	UFUNCTION()
		void OnClickedAudioBtn();

	UFUNCTION()
		void OnClickedLanguageBtn();

	UFUNCTION()
		void OnClickedControlBtn();

private:
	void OnOptionSelected(uint8 _SelectedNum);

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UButton> Btn_Graphics_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> Btn_Audio_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> Btn_Language_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> Btn_Control_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> Btn_Back_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UWidgetSwitcher> WST_Options_;

private:
	TArray<TObjectPtr<UButton>> OptionButtons_;
	uint8 SelectedNum_;
};
