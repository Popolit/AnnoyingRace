#include "LobbyPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RaceGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "UI/OptionWidget.h"
#include "UI/ExitDialogueWidget.h"
#include "UI/MessageDialogueWidget.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "World/RaceWorldSettings.h"

class URaceGameInstance;

ALobbyPlayerController::ALobbyPlayerController()
{
	SetActorTickEnabled(false);
	
	AudioComponent_ = CreateDefaultSubobject<UAudioComponent>("Audio");
	AudioComponent_->SetupAttachment(GetRootComponent());
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(LobbyWidgetClass_);

	LobbyWidget_ = CreateWidget(this, LobbyWidgetClass_);
	check(LobbyWidget_);

	//BGM재생
	if (ensureMsgf(SoundMix_, TEXT("Lobby Controller's SoundMix was not set")))
	{
		UGameplayStatics::PushSoundMixModifier(this, SoundMix_);
		auto WorldSettings = Cast<ARaceWorldSettings>(GetWorldSettings());
		if (ensureMsgf(WorldSettings && WorldSettings->WorldBGM_, TEXT("World Settings' BGM was not set")))
		{
			check(AudioComponent_);
			AudioComponent_->SetSound(WorldSettings->WorldBGM_);
			AudioComponent_->Play();
		}
	}
	WidgetStack_.Push(LobbyWidget_);
	LobbyWidget_->AddToViewport(0);

	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(LobbyWidget_->TakeWidget());
	SetInputMode(InputMode);
	bShowMouseCursor = true;

	//표기할 에러 메시지가 있으면 메시지 다이얼로그 오픈
	URaceGameInstance* GI = GetGameInstance<URaceGameInstance>();
	if (GI) 
	{
		GI->OnSessionError_.BindUObject(this, &ALobbyPlayerController::OpenMessageDialogue);
		if (false == GI->PendingErrorMessage_.IsEmpty())
		{
			OpenMessageDialogue(GI->PendingErrorMessage_);
			GI->PendingErrorMessage_ = FText::GetEmpty();
		}
	}
}

void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	auto IC = Cast<UEnhancedInputComponent>(InputComponent);
	if (ensureMsgf(IC, TEXT("Lobby's InputComponent was nullptr")))
	{
		auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(IMC_Default_, 0);
			if (auto UserSettings = Subsystem->GetUserSettings())
			{
				UserSettings->RegisterInputMappingContext(IMC_Character_);
			}
		}
		if (ensureMsgf(IA_Cancel_, TEXT("%s's IA_Cancel was nullptr"), *GetName()))
		{
			IC->BindAction(IA_Cancel_, ETriggerEvent::Triggered, this, &ALobbyPlayerController::OnCancelButtonPressed);
		}
	}
}

void ALobbyPlayerController::ExitGame()
{
	ConsoleCommand("quit");
}

void ALobbyPlayerController::OpenCreateSession()
{
	if (nullptr == CreateSessionWidget_)
	{
		if (ensureMsgf(CreateSessionWidgetClass_, TEXT("Lobby's CreateSession Widget was not set")))
		{
			CreateSessionWidget_ = CreateWidget(this, CreateSessionWidgetClass_);
		}
	}
	
	if (ensureMsgf(CreateSessionWidget_, TEXT("CreateSession Widget was nullptr")))
	{
		if (false == CreateSessionWidget_->IsInViewport())
		{
			CreateSessionWidget_->AddToViewport(1);
		}
		else
		{
			CreateSessionWidget_->SetVisibility(ESlateVisibility::Visible);
		}
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(CreateSessionWidget_->TakeWidget());
		SetInputMode(InputMode);
		WidgetStack_.Push(CreateSessionWidget_);
	}
}

void ALobbyPlayerController::CloseCreateSession()
{
	if (WidgetStack_.Top() == CreateSessionWidget_)
	{
		CloseTopWidget();
	}
	else
	{
		CreateSessionWidget_->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ALobbyPlayerController::OpenJoinSession()
{
	if (nullptr == JoinSessionWidget_)
	{
		if (ensureMsgf(JoinSessionWidgetClass_, TEXT("Lobby's JoinSession Widget was not set")))
		{
			JoinSessionWidget_ = CreateWidget(this, JoinSessionWidgetClass_);
		}
	}
	
	if (ensureMsgf(JoinSessionWidget_, TEXT("JoinSession Widget was nullptr")))
	{
		if (false == JoinSessionWidget_->IsInViewport())
		{
			JoinSessionWidget_->AddToViewport(1);
		}
		else
		{
			JoinSessionWidget_->SetVisibility(ESlateVisibility::Visible);
		}
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(JoinSessionWidget_->TakeWidget());
		SetInputMode(InputMode);
		WidgetStack_.Push(JoinSessionWidget_);
	}
}

void ALobbyPlayerController::CloseJoinSession()
{
	if (WidgetStack_.Top() == JoinSessionWidget_)
	{
		CloseTopWidget();
	}
	else
	{
		JoinSessionWidget_->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ALobbyPlayerController::OpenOption()
{
	if (nullptr == OptionWidget_)
	{
		if (ensureMsgf(OptionWidgetClass_, TEXT("Lobby's Option Widget was not set")))
		{
			OptionWidget_ = CreateWidget<UOptionWidget>(this, OptionWidgetClass_);
		}
	}
	
	if (ensureMsgf(OptionWidget_, TEXT("Option Widget was nullptr")))
	{
		if (false == OptionWidget_->IsInViewport())
		{
			OptionWidget_->OnBackBtnClicked_.BindUObject(this, &ALobbyPlayerController::CloseOption);
			OptionWidget_->AddToViewport(1);
		}
		else
		{
			OptionWidget_->SetVisibility(ESlateVisibility::Visible);
		}
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(OptionWidget_->TakeWidget());
		SetInputMode(InputMode);
		WidgetStack_.Push(OptionWidget_);
	}
}


void ALobbyPlayerController::OpenExitDialogue()
{
	if (nullptr == ExitDialogueWidget_)
	{
		if (ensureMsgf(ExitDialogueWidgetClass_, TEXT("Lobby's Exit Widget was not set")))
		{
			ExitDialogueWidget_ = CreateWidget<UExitDialogueWidget>(this, ExitDialogueWidgetClass_);
			ExitDialogueWidget_->OnYesBtnClicked_.BindUObject(this, &ALobbyPlayerController::ExitGame);
			ExitDialogueWidget_->OnNoBtnClicked_.BindUObject(this, &ALobbyPlayerController::CloseExitDialogue);
		}
	}
	
	if (ensureMsgf(ExitDialogueWidget_, TEXT("Exit Widget was nullptr")))
	{
		if (false == ExitDialogueWidget_->IsInViewport())
		{
			ExitDialogueWidget_->AddToViewport(2);
		}
		else
		{
			ExitDialogueWidget_->SetVisibility(ESlateVisibility::Visible);
		}
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(ExitDialogueWidget_->TakeWidget());
		SetInputMode(InputMode);
		WidgetStack_.Push(ExitDialogueWidget_);
	}
}

void ALobbyPlayerController::OpenMessageDialogue(const FText& _Message)
{
	if (nullptr == MessageDialogueWidget_)
	{
		if (ensureMsgf(MessageDialogueWidgetClass_, TEXT("Lobby's Dialogue Widget was not set")))
		{
			MessageDialogueWidget_ = CreateWidget<UMessageDialogueWidget>(this, MessageDialogueWidgetClass_);
			MessageDialogueWidget_->SetMessage(_Message);
			MessageDialogueWidget_->OnClickedClose_.BindUObject(this, &ALobbyPlayerController::CloseMessageDialogue);
		}
	}
	
	if (ensureMsgf(MessageDialogueWidget_, TEXT("Dialogue Widget was nullptr")))
	{
		if (false == MessageDialogueWidget_->IsInViewport())
		{
			MessageDialogueWidget_->AddToViewport(2);
			MessageDialogueWidget_->SetMessage(_Message);
		}
		else
		{
			MessageDialogueWidget_->SetVisibility(ESlateVisibility::Visible);
		}
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(MessageDialogueWidget_->TakeWidget());
		SetInputMode(InputMode);
		WidgetStack_.Push(MessageDialogueWidget_);
	}
}


void ALobbyPlayerController::CloseOption()
{
	if (WidgetStack_.Top() == OptionWidget_)
	{
		CloseTopWidget();
	}
	else
	{
		OptionWidget_->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ALobbyPlayerController::CloseExitDialogue()
{
	if (WidgetStack_.Top() == ExitDialogueWidget_)
	{
		CloseTopWidget();
	}
	else
	{
		ExitDialogueWidget_->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ALobbyPlayerController::CloseMessageDialogue()
{
	if (WidgetStack_.Top() == MessageDialogueWidget_)
	{
		CloseTopWidget();
	}
	else
	{
		MessageDialogueWidget_->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ALobbyPlayerController::OnCancelButtonPressed()
{
	if (WidgetStack_.Top() == LobbyWidget_)
	{
		OpenExitDialogue();
		return;
	}
	CloseTopWidget();
}

//가장 위에 있는 위젯을 닫고, Top 위젯에게 포커스
void ALobbyPlayerController::CloseTopWidget()
{
	if (WidgetStack_.IsEmpty())
	{
		ensureMsgf(false, TEXT("Widget Stack was Empty!"));
		return;
	}
	if (WidgetStack_.Top() == LobbyWidget_)
	{
		ensureMsgf(false, TEXT("Trying to Close Lobby Widget"));
		return;
	}
	
	WidgetStack_.Top()->SetVisibility(ESlateVisibility::Collapsed);
	WidgetStack_.Pop();

	if (WidgetStack_.IsEmpty())
	{
		ensureMsgf(false, TEXT("Widget Stack was Empty!"));
		return;
	}
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(WidgetStack_.Top()->TakeWidget());
	SetInputMode(InputMode);
}


