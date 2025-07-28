#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RacePlayerController.generated.h"

/**
 * Race PlayerController
 */
UCLASS()
class ANNOYINGRACE_API ARacePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ARacePlayerController();

protected:
	virtual void OnPossess(APawn* _Pawn) override;

	virtual void OnUnPossess() override;

	virtual void SetupInputComponent() override;

public:
	void PlaySequence(const FName& _SequenceName);

	//Transform에서 관전자 모드
	void SetSpectatorMode(const FTransform& _TransformToSpectate);

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
	void OpenMenu();

	UFUNCTION(Server, Reliable)
		void Server_RequestDrawCharacter();

	UFUNCTION(Server, Reliable)
		void Server_RequestSpawnCharacter();

	void OnDrawAnimationFinished();

	void OnCountDownAnimationFinished();

	void OnCharacterDiedAnimationFinished();

	void UpdateDistanceAlongSpline();

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCountDownWidget> CountDownWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UDrawCharacterWidget> DrawCharacterWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCharacterDiedWidget> CharacterDiedWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> PlayerHUDWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> RaceMenuWidgetClass_;

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

private:
	UPROPERTY()
		TObjectPtr<class ATrackSplineActor> TrackSplineActor_;

	//캐릭터 인풋
	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<class UInputMappingContext> IMC_Character_;

	//공용 인풋 (Ex : 메뉴 열기 등)
	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<class UInputMappingContext> IMC_Default_;

	UPROPERTY(EditDefaultsOnly, Category = Input)
		TObjectPtr<class UInputAction> IA_ToggleMenu_;

	FTimerHandle DistanceUpdateTimerHandle_;

	bool bEnableCharacterInput_ = false;
};
