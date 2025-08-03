#include "Option_GraphicsWidget.h"

#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

void UOption_GraphicsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();

	if(ensureMsgf(UserSettings, TEXT("UserSettings was nullptr")))
	{
		FIntPoint Resolution = UserSettings->GetScreenResolution();
		CBB_Resolution_->SetSelectedOption(FString::FromInt(Resolution.X) + " x " + FString::FromInt(Resolution.Y));

		const auto Mode = UserSettings->GetFullscreenMode();

		if(Mode ==  EWindowMode::Fullscreen)
		{
			CBB_FullScreen_->SetSelectedOption("Full Screen");
		}
		else if (Mode == EWindowMode::WindowedFullscreen)
		{
			CBB_FullScreen_->SetSelectedOption("Full Screen(Borderless)");
		}
		else if(Mode == EWindowMode::Windowed)
		{
			CBB_FullScreen_->SetSelectedOption("Windowed");
		}
	}

	CBB_Resolution_->OnSelectionChanged.AddDynamic(this, &UOption_GraphicsWidget::OnResolutionChanged);
	CBB_FullScreen_->OnSelectionChanged.AddDynamic(this, &UOption_GraphicsWidget::OnFullScreenModeChanged);
}

void UOption_GraphicsWidget::OnResolutionChanged(FString _SelectedString, ESelectInfo::Type _SelectionType)
{
	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();

	if (nullptr == UserSettings)
	{
		return;
	}

	FString X;
	FString Y;
	_SelectedString.Split(" x ", &X , &Y);
	UserSettings->SetScreenResolution({ FCString::Atoi(*X) , FCString::Atoi(*Y) });
	UserSettings->ApplyResolutionSettings(false);
}

void UOption_GraphicsWidget::OnFullScreenModeChanged(FString _SelectedString, ESelectInfo::Type _SelectionType)
{
	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();

	if (nullptr == UserSettings)
	{
		return;
	}

	if(_SelectedString == "Full Screen")
	{
		UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
	}
	else if (_SelectedString == "Full Screen(Borderless)")
	{
		UserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
	}
	else if(_SelectedString ==  "Windowed")
	{
		UserSettings->SetFullscreenMode(EWindowMode::Windowed);
	}
	UserSettings->ApplyResolutionSettings(false);
}
