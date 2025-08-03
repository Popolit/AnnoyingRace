#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaceMenuWidget.generated.h"

/**
 * Race Menu UI
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API URaceMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	URaceMenuWidget(const FObjectInitializer& _ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& _InGeometry, const FKeyEvent& _InKeyEvent) override;

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UButton> Btn_Resume_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> Btn_Option_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> Btn_Exit_;
};
