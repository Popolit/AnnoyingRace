#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Option_GraphicsWidget.generated.h"

/**
 * Graphics Option Widget
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API UOption_GraphicsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
		void OnResolutionChanged(FString _SelectedString, ESelectInfo::Type SelectionType);

	UFUNCTION()
		void OnFullScreenModeChanged(FString _SelectedString, ESelectInfo::Type SelectionType);
private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UComboBoxString> CBB_Resolution_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UComboBoxString> CBB_FullScreen_;
};
