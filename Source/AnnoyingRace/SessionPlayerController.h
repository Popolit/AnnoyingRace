#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SessionPlayerController.generated.h"

/**
 * Session PlayerController
 */
UCLASS(HideDropdown)
class ANNOYINGRACE_API ASessionPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ASessionPlayerController();
	
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

public:
	UFUNCTION(Server, Reliable)
		void Server_RequestToggleReady();

	UFUNCTION(Server, Reliable)
		void Server_RequestMakeSessionPublic();

	UFUNCTION(Server, Reliable)
		void Server_SendChat(const FText& _Chat);

	UFUNCTION(Client, Reliable)
		void Client_AddChatOnLog(const FText& _Chat, const FString& _ChatterName);

	UFUNCTION(Client, Reliable)
		void Client_StartCountDownForPlay();

	UFUNCTION(Client, Reliable)
		void Client_StopCountDownForPlay();

	void RequestSetSessionJoinable(bool _bCanJoin);
	
	void RequestServerTravel(const FPrimaryAssetId& _MapId);

private:
	void OpenMessageDialogue(const FText& _Message);

	void CloseMessageDialogue();
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		TObjectPtr<class UAudioComponent> AudioComponent_;
	
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class USessionWidget> SessionWidgetClass_;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UMessageDialogueWidget> MessageDialogueWidgetClass_;

	UPROPERTY()
		TObjectPtr<USessionWidget> SessionWidget_;

	UPROPERTY()
		TObjectPtr<UMessageDialogueWidget> MessageDialogueWidget_;
	
	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<class UInputMappingContext> IMC_Default_;

	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<class UInputAction> IA_Cancel_;

	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<USoundMix> SoundMix_;
};
