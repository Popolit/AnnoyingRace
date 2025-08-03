#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeyMappingSlotWidget.generated.h"

/**
 * KeyMappingSlot Widget
 */
UCLASS()
class ANNOYINGRACE_API UKeyMappingSlotWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	void SetWidgetSettings(const FName& _MappingName, class UEnhancedInputUserSettings* _UserSettings);

private:
	UFUNCTION()
		void OnKeySelected(FInputChord _SelectedKey);

private:
	UPROPERTY(meta=(BindWidget))
		TObjectPtr<class UTextBlock> Txt_KeyName_;
		
	UPROPERTY(meta=(BindWidget))
		TObjectPtr<class UInputKeySelector> IKS_Key_;

	UPROPERTY()
		TObjectPtr<UEnhancedInputUserSettings> UserSettings_;
		
	FName MappingName_;
};
