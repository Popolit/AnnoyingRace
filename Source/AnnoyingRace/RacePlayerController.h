#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RacePlayerController.generated.h"

/**
 * Race PlayerController
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API ARacePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void OnPossess(APawn* _Pawn) override;

	virtual void OnUnPossess() override;

	virtual void SetupInputComponent() override;

public:
	void PlaySequence(const FName& _SequenceName);

	UFUNCTION(Client, Reliable)
	void Client_PlaySound2D(class USoundCue* _Sound);

	//Transform에서 관전자 모드 시작
	void SetSpectatorMode(const FTransform& _TransformToSpectate);

	//UI관련 함수들
public:
	void OpenMainMenu();

	UFUNCTION()
		void ExitGame();

	UFUNCTION()
		void CloseMainMenu();

	UFUNCTION()
		void OpenOptionMenu();

	UFUNCTION()
		void OpenExitDialogue();

private:
	void CloseOptionMenu();

	void CloseExitDialogue();

	//Client 함수들
public:
	UFUNCTION(Client, Reliable)
		void Client_EnableCharacterInput();

	UFUNCTION(Client, Reliable)
		void Client_DisableCharacterInput();

	UFUNCTION(Client, Reliable)
		void Client_ShowCharacterDiedWidget();

	UFUNCTION(Client, Reliable)
		void Client_StartRaceCountdown();

	UFUNCTION(Client, Reliable)
		void Client_ShowCharacterDrawResult(const class UCharacterData* _DrawnCharacterData);

private:
	UFUNCTION(Server, Reliable)
		void Server_RequestDrawCharacter();

	UFUNCTION(Server, Reliable)
		void Server_RequestSpawnCharacter();

	void OnDrawAnimationFinished();

	void OnCountDownAnimationFinished();

	void OnCharacterDiedAnimationFinished();

	void UpdateDistanceAlongSpline();

	void OpenInteractableWidget(UUserWidget* _Widget);

	void CloseInteractableWidget(UUserWidget* _Widget);

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCountDownWidget> CountDownWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UDrawCharacterWidget> DrawCharacterWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCharacterDiedWidget> CharacterDiedWidgetClass_;

	UPROPERTY(EditDefaultsOnly, meta=(AllowedClasses="PlayerHUDWidget"))
		TSubclassOf<UUserWidget> PlayerHUDWidgetClass_;

	UPROPERTY(EditDefaultsOnly, meta=(AllowedClasses="RaceMenuWidget"))
		TSubclassOf<UUserWidget> RaceMenuWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UOptionWidget> OptionWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UExitDialogueWidget> ExitDialogueWidgetClass_;

	UPROPERTY()
		TObjectPtr<UCountDownWidget> CountdownWidget_;

	UPROPERTY()
		TObjectPtr<UDrawCharacterWidget> DrawCharacterWidget_;

	UPROPERTY()
		TObjectPtr<UCharacterDiedWidget> CharacterDiedWidget_;

	UPROPERTY()
		TObjectPtr<UUserWidget> PlayerHUDWidget_;

	UPROPERTY()
		TObjectPtr<UUserWidget> RaceMenuWidget_;

	UPROPERTY()
		TObjectPtr<UOptionWidget> OptionWidget_;

	UPROPERTY()
		TObjectPtr<UExitDialogueWidget> ExitDialogueWidget_;

private:
	UPROPERTY()
		TObjectPtr<class ATrackSplineActor> TrackSplineActor_;

	//캐릭터 인풋
	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> IMC_Character_;

	//공용 인풋 (Ex : 메뉴 열기 등)
	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<UInputMappingContext> IMC_Default_;

	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<class UInputAction> IA_ToggleMenu_;

	FTimerHandle DistanceUpdateTimerHandle_;

	bool bEnableCharacterInput_ = false;
};
