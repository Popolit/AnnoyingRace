#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConfirmExitGameWidget.generated.h"

/**
 * Confirm Exit Game Dialog Widget
 */
UCLASS()
class ANNOYINGRACE_API UConfirmExitGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UConfirmExitGameWidget(const FObjectInitializer& _ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UButton> Btn_Yes_;

	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> Btn_No_;
};
