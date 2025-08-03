#include "Option_LanguageWidget.h"

#include "Components/ComboBoxString.h"
#include "World/RaceGameUserSettings.h"

void UOption_LanguageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto Settings = Cast<URaceGameUserSettings>(GEngine->GetGameUserSettings());
	if (Settings)
	{
		if (Settings->Language_ == TEXT("en"))
		{
			CBB_Language_->SetSelectedOption(TEXT("English"));
		}
		else if (Settings->Language_ == TEXT("ko"))
		{
			CBB_Language_->SetSelectedOption(TEXT("한국어"));
		}
	}
	CBB_Language_->OnSelectionChanged.AddDynamic(this, &UOption_LanguageWidget::OnSelectionChanged);
}

void UOption_LanguageWidget::OnSelectionChanged(FString _SelectedItem, ESelectInfo::Type _SelectionType)
{
	FString Language;
	if (_SelectedItem == TEXT("English"))
	{
		Language = TEXT("en");
	}
	else if (_SelectedItem == TEXT("한국어"))
	{
		Language = TEXT("ko");
	}

	if (!Language.IsEmpty())
	{
		auto Settings = Cast<URaceGameUserSettings>(GEngine->GetGameUserSettings());
		if (Settings)
		{
			Settings->Language_ = Language;
			FInternationalization::Get().SetCurrentCulture(Language);
			Settings->ApplySettings(false);
			Settings->SaveSettings();
		}
	}
}
