#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageDialogueWidget.generated.h"

DECLARE_DELEGATE(FOnClickedClose);

/**
 * Message Dialogue Widget
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API UMessageDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UMessageDialogueWidget(const FObjectInitializer& _ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& _InGeometry, const FKeyEvent& _InKeyEvent) override;

public:
	void SetMessage(const FText& _MessageKey) const;

private:
	UFUNCTION()
		void OnClickedClose();
	
private:
	UPROPERTY(meta=(BindWidget))
		TObjectPtr<class UTextBlock> Txt_Message_;
	
	UPROPERTY(meta=(BindWidget))
		TObjectPtr<class UButton> Btn_Close_;

	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<UStringTable> ST_Messages_;

public:
	FOnClickedClose OnClickedClose_;
};
