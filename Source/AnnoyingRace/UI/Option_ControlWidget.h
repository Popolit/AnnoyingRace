#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Option_ControlWidget.generated.h"

/**
 * Control Option Widget
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API UOption_ControlWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:

	//Buttons
private:
	UPROPERTY(meta=(BindWidget))
		TObjectPtr<class UScrollBox> SCB_Keys_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UKeyMappingSlotWidget> SlotClass_;
};
