#include "Option_ControlWidget.h"

#include "EnhancedInputSubsystems.h"
#include "KeyMappingSlotWidget.h"
#include "Components/ScrollBox.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UOption_ControlWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SCB_Keys_->ClearChildren();

	if (nullptr == SlotClass_)
	{
		ensureMsgf(false, TEXT("SlotClass was not set"));
		return;
	}
	
	APlayerController* PC = GetOwningPlayer();
	if (nullptr == PC)
	{
		ensure(false);
		return;
	}

	ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
	if (nullptr == LocalPlayer)
	{
		ensure(false);
		return;
	}

	auto InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(nullptr == InputSubsystem)
	{
		ensureMsgf(false, TEXT("InputSystem was not set"));
		return;
	}
	UEnhancedInputUserSettings* UserSettings = InputSubsystem->GetUserSettings();

	if(ensureMsgf(UserSettings, TEXT("Enhnaced Input UserSetting was not set")))
	{
		auto Profile = UserSettings->GetCurrentKeyProfile();
		for (auto& Elem : Profile->GetPlayerMappingRows())
		{
			auto Widget = CreateWidget<UKeyMappingSlotWidget>(this, SlotClass_);
			Widget->SetWidgetSettings(Elem.Key, UserSettings);
			SCB_Keys_->AddChild(Widget);
		}
	}
}

