#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * Lobby PlayerController
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyPlayerController();
	
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

public:
	UFUNCTION()
		void ExitGame();
	
	UFUNCTION()
		void OpenCreateSession();

	UFUNCTION()
		void CloseCreateSession();

	UFUNCTION()
		void OpenJoinSession();

	UFUNCTION()
		void CloseJoinSession();
	
	UFUNCTION()
		void OpenOption();

	UFUNCTION()
		void OpenExitDialogue();

	UFUNCTION()
		void OpenMessageDialogue(const FText& _Message);

private:
	void CloseOption();

	void CloseExitDialogue();

	void CloseMessageDialogue();
	
	void OnCancelButtonPressed();

	void CloseTopWidget();

private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowedClasses = "LobbyWidget"))
		TSubclassOf<UUserWidget> LobbyWidgetClass_;

	UPROPERTY(EditDefaultsOnly, meta=(AllowedClasses = "CreateSessionWidget"))
		TSubclassOf<UUserWidget> CreateSessionWidgetClass_;

	UPROPERTY(EditDefaultsOnly, meta=(AllowedClasses = "JoinSessionWidget"))
		TSubclassOf<UUserWidget> JoinSessionWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UOptionWidget> OptionWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UExitDialogueWidget> ExitDialogueWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UMessageDialogueWidget> MessageDialogueWidgetClass_;
	
	UPROPERTY()
		TObjectPtr<UUserWidget> LobbyWidget_;

	UPROPERTY()
		TObjectPtr<UUserWidget> CreateSessionWidget_;

	UPROPERTY()
		TObjectPtr<UUserWidget> JoinSessionWidget_;
	
	UPROPERTY()
		TObjectPtr<UOptionWidget> OptionWidget_;

	UPROPERTY()
		TObjectPtr<UExitDialogueWidget> ExitDialogueWidget_;
	
	UPROPERTY()
		TObjectPtr<UMessageDialogueWidget> MessageDialogueWidget_;
	
private:
	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<class UInputMappingContext> IMC_Default_;

	//로비에서 사용하지는 않으나, 옵션 매핑을 위한 컨텍스트
	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<UInputMappingContext> IMC_Character_;

	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<class UInputAction> IA_Cancel_;

	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<USoundMix> SoundMix_;
	
	TArray<UUserWidget*> WidgetStack_;
};
