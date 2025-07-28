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
	UFUNCTION()
		void ResumeGame();

	UFUNCTION()
		void VisibilityChanged(ESlateVisibility _Visibility);

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UButton> Btn_Resume_;
};
