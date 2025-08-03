#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Option_AudioWidget.generated.h"

/**
 * Audio Option Widget
 */
UCLASS()
class ANNOYINGRACE_API UOption_AudioWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
		void OnMasterValueChanged(float _Value);
	UFUNCTION()
		void OnSFXValueChanged(float _Value);
	UFUNCTION()
		void OnBGMValueChanged(float _Value);

private:
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class USlider> Sld_Master_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UTextBlock> Txt_MasterPercent_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<USlider> Sld_SFX_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> Txt_SFXPercent_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<USlider> Sld_BGM_;
	UPROPERTY(meta = (BindWidget))
		TObjectPtr<UTextBlock> Txt_BGMPercent_;

private:
	UPROPERTY(EditAnywhere, Category = "Audio Settings")
		TObjectPtr<USoundMix> GlobalSoundMix;

	UPROPERTY(EditAnywhere, Category = "Audio Settings")
		TObjectPtr<USoundClass> MasterSoundClass;

	UPROPERTY(EditAnywhere, Category = "Audio Settings")
		TObjectPtr<USoundClass> SFXSoundClass;

	UPROPERTY(EditAnywhere, Category = "Audio Settings")
		TObjectPtr<USoundClass> BGMSoundClass;
};
