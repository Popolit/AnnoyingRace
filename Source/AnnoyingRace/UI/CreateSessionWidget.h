#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreateSessionWidget.generated.h"

/**
 * Create Session Widget
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API UCreateSessionWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
		void OnCheckPrivateStateChanged(bool _bChecked);
	
	UFUNCTION()
		void OnMaxUserCountChanged(const FText& _Text);

	UFUNCTION()
		void OnMaxUserCountCommitted(const FText& _Text, ETextCommit::Type _CommitType);

	UFUNCTION()
		void OnClickedCreateBtn();

	void OnMapsLoaded();
	
	bool CheckAllPropertyIsValid(FText& _ErrorMessage);

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UEditableTextBox> ETB_SessionName_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UEditableTextBox> ETB_Password_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UCheckBox> CHB_IsPrivate_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UEditableTextBox> ETB_MaxUserCount_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UComboBoxString> CBB_MapList_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UButton> Btn_Cancel_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> Btn_Create_;

private:
	FText LastValidUserCount_;
};
