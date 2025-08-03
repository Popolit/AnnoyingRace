#include "KeyMappingSlotWidget.h"

#include "Components/InputKeySelector.h"
#include "Components/TextBlock.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UKeyMappingSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	IKS_Key_->OnKeySelected.AddDynamic(this, &UKeyMappingSlotWidget::OnKeySelected);
}

void UKeyMappingSlotWidget::SetWidgetSettings(const FName& _MappingName, UEnhancedInputUserSettings* _UserSettings)
{
	MappingName_ = _MappingName;
	UserSettings_ = _UserSettings;
	
	if (UserSettings_)
	{
		auto Profile = UserSettings_->GetCurrentKeyProfile();
		const FKeyMappingRow* KeyMappingRow = Profile->FindKeyMappingRow(MappingName_);

		if (KeyMappingRow)
		{
			for (auto& Elem : KeyMappingRow->Mappings)
			{
				Txt_KeyName_->SetText(Elem.GetDisplayName());
				IKS_Key_->SetSelectedKey(Elem.GetCurrentKey());
			}
		}
	}
}

void UKeyMappingSlotWidget::OnKeySelected(FInputChord _SelectedKey)
{
	if (UserSettings_)
	{
		FMapPlayerKeyArgs Args;
		Args.MappingName = MappingName_;
		Args.Slot = EPlayerMappableKeySlot::First;
		Args.NewKey = _SelectedKey.Key;

		FGameplayTagContainer FailureReason;
		UserSettings_->MapPlayerKey(Args, FailureReason);
		UserSettings_->SaveSettings();
	}
}
