#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaitingPlayersWidget.generated.h"

/**
 * Waiting Players After Begin Play
 */
UCLASS()
class ANNOYINGRACE_API UWaitingPlayersWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta=(BindWidget))
		TObjectPtr<class UTextBlock> Txt_ReadiedCount_;
	UPROPERTY(meta=(BindWidget))
		TObjectPtr<UTextBlock> Txt_TotalCount_;
};
