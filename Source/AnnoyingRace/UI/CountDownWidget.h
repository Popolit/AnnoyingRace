#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountDownWidget.generated.h"

/**
 * Count Down when Race Begin
 */
UCLASS()
class ANNOYINGRACE_API UCountDownWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	void UpdateCountDown();

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> TxtCountDown_;

	UINT CountDown = 3;
};
