#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Option_LanguageWidget.generated.h"

/**
 * Language Option Widget
 */
UCLASS()
class ANNOYINGRACE_API UOption_LanguageWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
		void OnSelectionChanged(FString _SelectedItem, ESelectInfo::Type _SelectionType);
private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UComboBoxString> CBB_Language_;
};
