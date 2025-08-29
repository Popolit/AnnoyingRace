#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExitDialogueWidget.generated.h"

DECLARE_DELEGATE(FOnBtnClicked);

/**
 * LobbyExitDialogue Widget
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API UExitDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UExitDialogueWidget(const FObjectInitializer& _ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
		void OnClckedYesBtn_();

	UFUNCTION()
		void OnClckedNoBtn_();

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Yes_;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn_No_;

public:
	FOnBtnClicked OnYesBtnClicked_;
	FOnBtnClicked OnNoBtnClicked_;
};
