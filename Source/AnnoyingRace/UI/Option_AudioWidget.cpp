#include "Option_AudioWidget.h"

#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "World/RaceGameUserSettings.h"

void UOption_AudioWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Txt_MasterPercent_->SetText(FText::AsNumber(FMath::RoundToInt(Sld_Master_->GetValue())));

	Sld_Master_->OnValueChanged.AddDynamic(this, &UOption_AudioWidget::OnMasterValueChanged);
	Sld_SFX_->OnValueChanged.AddDynamic(this, &UOption_AudioWidget::OnSFXValueChanged);
	Sld_BGM_->OnValueChanged.AddDynamic(this, &UOption_AudioWidget::OnBGMValueChanged);

	URaceGameUserSettings* Settings = Cast<URaceGameUserSettings>(GEngine->GetGameUserSettings());
	if (ensureMsgf(Settings, TEXT("GameUserSettings' class was not set")))
	{
		Sld_Master_->SetValue(Settings->MasterVolume);
		Sld_SFX_->SetValue(Settings->SFXVolume);
		Sld_BGM_->SetValue(Settings->BGMVolume);
		UGameplayStatics::SetSoundMixClassOverride(this, GlobalSoundMix, MasterSoundClass, Settings->MasterVolume);
		UGameplayStatics::SetSoundMixClassOverride(this, GlobalSoundMix, SFXSoundClass, Settings->SFXVolume);
		UGameplayStatics::SetSoundMixClassOverride(this, GlobalSoundMix, BGMSoundClass, Settings->BGMVolume);
		Txt_MasterPercent_->SetText(FText::AsNumber(FMath::RoundToInt(Settings->MasterVolume * 100)));
		Txt_SFXPercent_->SetText(FText::AsNumber(FMath::RoundToInt(Settings->SFXVolume * 100)));
		Txt_BGMPercent_->SetText(FText::AsNumber(FMath::RoundToInt(Settings->BGMVolume * 100)));
	}
}

void UOption_AudioWidget::OnMasterValueChanged(float _Value)
{
	Txt_MasterPercent_->SetText(FText::AsNumber(FMath::RoundToInt(_Value * 100)));
	UGameplayStatics::SetSoundMixClassOverride(this, GlobalSoundMix, MasterSoundClass, _Value);
	URaceGameUserSettings* Settings = Cast<URaceGameUserSettings>(GEngine->GetGameUserSettings());
	if (Settings)
	{
		Settings->MasterVolume = _Value;
	}
}

void UOption_AudioWidget::OnSFXValueChanged(float _Value)
{
	Txt_SFXPercent_->SetText(FText::AsNumber(FMath::RoundToInt(_Value * 100)));
	UGameplayStatics::SetSoundMixClassOverride(this, GlobalSoundMix, SFXSoundClass, _Value);
	URaceGameUserSettings* Settings = Cast<URaceGameUserSettings>(GEngine->GetGameUserSettings());
	if (Settings)
	{
		Settings->SFXVolume = _Value;
	}
}

void UOption_AudioWidget::OnBGMValueChanged(float _Value)
{
	Txt_BGMPercent_->SetText(FText::AsNumber(FMath::RoundToInt(_Value * 100)));
	UGameplayStatics::SetSoundMixClassOverride(this, GlobalSoundMix, BGMSoundClass, _Value);
	URaceGameUserSettings* Settings = Cast<URaceGameUserSettings>(GEngine->GetGameUserSettings());
	if (Settings)
	{
		Settings->BGMVolume = _Value;
	}
}
